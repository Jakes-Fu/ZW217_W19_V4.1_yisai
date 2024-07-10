/******************************************************************************
 ** File Name:      fdl_main.c                                                 *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           01/01/2012                                                *
 ** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
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
#include "fdl_stdio.h"
#include "sio_drv.h"
#include "fdl_crc.h"
#include "fdl_main.h"
#include "flash_drv.h"
#include "packet.h"
#include "usb_boot.h"
#include "sci_types.h"
#include "sc_fdl_reg.h"
#include "virtual_com.h"
#include "update_fixnv.h"
#include "flash.h"
#include "detect_error.h"
#include "fdl_channel.h"
#include "dl_engine.h"
#include "umem_main.h"
#include "umem_cfg.h"
#include "flash_drvapi.h"
#include "sfc_drvapi.h"
#include "mem_cfg.h"

//#include "iomux_6121e.h"      //w217
#include "ap_clk.h"             //w217
#include "ap_apb.h"             //w217
//#include "spi_flash_6121e.h"  //w217
#include "sfc_reg_uws6121e.h"

#ifdef FLASH_TYPE_SPI
#include "spiflash.h"
#endif

#ifdef UMEM_SUPPORT
#ifdef UMEM_NEW_MECHNISM
#include "nv_item_id.h"
#endif
#endif

#ifdef SECURE_BOOT_SUPPORT
#include "hal_rom_api.h"
#endif

#include "watchdog_phy.h"
#include "watchdog_reg_v5.h"
#if defined(CHIP_VER_6531) || defined(CHIP_VER_6531EFM)
#include "smc_drvapi.h"
#endif

//#define FIXNV_SECTOR_NUM  3

#define REG32(x)                                    (*((volatile uint32 *)(x)))
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
extern void JumpToFDL (int StartAddr);
extern void get_flash_id (uint32 *pManufacturerID, uint32 *pDeviceID, uint32 *pExtendID);
extern NORMCP_ENTITY_PTR NORMCP_GetEntity (uint32 manu_id, uint32 devc_id, uint32 extend_id, uint32 *index);
extern uint32 get_valid_nv_address (void);
extern void initialize_nv_status (NOR_FLASH_CONFIG_PTR ptr,uint32 base_address);
extern uint32 update_productinfo (char *prod_address,uint32 write_size);
extern uint32 update_fixnv (char *fix_nv_address,uint32 fix_nv_size);
extern unsigned long Get_CheckSum (const unsigned char *src, int len);
NOR_FLASH_CONFIG_PTR get_flash_partition_info_InPS (void);
extern uint32 CHIP_PHY_Get7sRstFlag (void);
extern void CHIP_PHY_Clean7sRstFlag (void);
uint32 calculate_fixNV_length (uint32  start_addr, uint32 end_addr);
extern uint32    s_intel_m18_serial;

static uint16 flash_get_checksum (uint32 addr, uint32 len);
#if  defined(MEM_STRESS_TEST_NORFLASH)
void  norflash_test(uint32 start, uint32 end);
#endif
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define MAX_FLASH_COUNT 500
#define STONE_IMAGE_MAX_SIZE    (32*1024*1024)
#define MAX_SECARCH_RANGE       0x1000
//
// Memory Define
//
typedef struct
{
    uint32   start_addr;  // == 0xFFFFFFFF means reach tail
    uint32   size;        // == 0
} FLASH_INFOR_T;

#ifdef SECURE_BOOT_SUPPORT
volatile uint32 loop_var1=0;
#endif
//#ifdef SECURE_BOOT_SUPPORT
#if 0
#pragma arm section zidata = "VLR_DATA"
uint8 signature_data[512];
#pragma arm section zidata
#endif

#if defined(PLATFORM_SC6531EFM)
static const FLASH_INFOR_T  flash_infor[]=
{
    {0x10000000, 0x4000000},  // the max we support is 64M
    {0x14000000, 0x4000000},  // the max we support is 64M
    {0x18000000, 0x4000000},  // the max we support is 64M
    {0x1c000000, 0x4000000},  // the max we support is 64M
    {0xFFFFFFFF, 0x0}  // last one, indicate tail
};
#elif defined(PLATFORM_UWS6121E)
static const FLASH_INFOR_T  flash_infor[]=
{
    {PHY_FLASH_START_ADDR, PHY_FLASH_SIZE},  // FLASH
#ifdef MULTI_FLASH_SUPPORT
    {PHY_FLASH_START_ADDR_B, PHY_FLASH_SIZE_B},  // FLASH B
#endif
    {PHY_RAM_BASE_ADDR, MEM_RAM_SIZE},  // DDR or PSRAM
    {0xFFFFFFFF, 0x0}  // last one, indicate tail
};
#else
static const FLASH_INFOR_T  flash_infor[]=
{
    {0x30000000, 0x4000000},  // the max we support is 64M
    {0x34000000, 0x4000000},  // the max we support is 64M
    {0x38000000, 0x4000000},  // the max we support is 64M
    {0x3c000000, 0x4000000},  // the max we support is 64M
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

#if defined(PLATFORM_UWS6121E)
PUBLIC void SCI_PAssert (
    void    *exp,     // Expression (including pointers) that evaluates
    // to nonzero or 0
    void    *file,    // File name where assert
    uint32  line,      // Line number where assert
    char    *assert_info_ptr
)
{
}

PUBLIC char *SCI_SaveAssertInfo (
    const char *x_format, ...)
{
    return NULL;
}
#endif

sys_stage_s      global_sys_stage;
const char       version_string[] = {"Spreadtrum Boot Block version 1.2"};

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define ANA_REG_OR(reg_addr, value)  ADI_Analogdie_reg_or(reg_addr, value)
#endif

#define NOR_BOOTLOADER_LOGIC_ADRESS   0x80000000
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
#define FOTA_BOOTLOADER_LOGIC_ADRESS   0x80000002
#define FOTA_PACKAGE_LOGIC_ADDRESS     0x8000000f
#endif
#define NV_LOGIC_ADRESS               0x90000001
//when bak nv is failure,be used to recover fix NV with ResearchDownload tool.
//write all fix nv(inclue production informaiton)
#define NV_ALL_LOGIC_ADRESS           0x90001000
#define PS_LOGIC_ADRESS               0x80000003
#define RES_LOGIC_ADRESS              0x90000004
#define ERASEFLASH_LOGIC_ADRESS       0x90000003
#define PHASECHECK_LOGIC_ADRESS       0x90000002
#define UMEM_LOGIC_ERASE_ADRESS       0x90000005
#define UMEM_LOGIC_LOAD_ADRESS        0x90000006
#define RUNNING_NV_LOGIC_LOCAD_ADRESS 0x90000007
#define SPLOAD_LOGIC_LOAD_ADRESS      0x90000008
#define DELTA_NV_LOGIC_LOAD_ADRESS  0x90000100
#define CP_CODE_LOGIC_LOAD_ADRESS      0x90000101
#define TFBOOT_LOGIC_LOAD_ADRESS      0x90000110
#define APN_LOGIC_LOAD_ADRESS         0x90000111
#define IMS_TOGGLE_LOGIC_ERASE_ADRESS      0x90000112
#define CONFIG_NV_LOGIC_ERASE_ADRESS         0x90000113

#ifdef DSP_DOWNLOAD_SUPPORT
#define DSP_LOGIC_LOAD_ADRESS         0x90000009
#endif

#if defined(PLATFORM_SC6531EFM)
#define FLASH_START_ADRESS            0x10000000
#define FLASH_START_ADRESS_CS3        0x1C000000
#ifdef SECURE_BOOT_SUPPORT
#ifdef FOTA_SUPPORT_NONE
#define FLASH_CFG_OFFSET              (0x10000+0x400+0x64)//0x64+0x39C  // header space added in kernel for storing secure boot data
#else
#define FLASH_CFG_OFFSET              (0x20000+0x400+0x64)//0x64+0x39C  // header space added in kernel for storing secure boot data
#endif
#else
#define FLASH_CFG_OFFSET              0x64
#endif
#elif defined(PLATFORM_UWS6121E)
#define FLASH_PS_START_ADRESS       AP_BASE_ADDR
#define FLASH_START_ADRESS            0x00000000  //actual address 0x60000000
#define FLASH_START_ADRESS_CS3     0x1C000000
#ifdef SECURE_BOOT_SUPPORT
#define FLASH_CFG_OFFSET              (0x64+0x100)
#else
#define FLASH_CFG_OFFSET              0x64
#endif

#else
#define FLASH_START_ADRESS            0x30000000
#define FLASH_START_ADRESS_CS3        0x3C000000
#ifdef SECURE_BOOT_SUPPORT
#ifdef FOTA_SUPPORT_NONE
#define FLASH_CFG_OFFSET              (0x10000+0x400+0x60)//0x60+0x3A0 // header space added in kernel for storing secure boot data
#else
#define FLASH_CFG_OFFSET              (0x20000+0x400+0x60)//0x60+0x3A0 // header space added in kernel for storing secure boot data
#endif
#else
#define FLASH_CFG_OFFSET              0x60
#endif
#endif

#define CHECKSUM_OTHER_DATA           0x5555aaaa
#define ERR_NV_LEN                    0xffff
#define ITEM_MASK_ID                  0x1fff
#define EFS_INVALID_ITEM              0xffff
#define UMEM_ERASE_FLAG               0x66
#define MAX_PATITION_NUM              16
#define UMEM_IMG_VALID_FLAG           0xAABB
#define UMEM_IMG_INFO_SIZE            0x6

#define M18_REGION_SIZE               1024

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define SRAM_UMEM_START_ADDR          0x14180000
#else
#define SRAM_UMEM_START_ADDR          0x34180000
#endif

//@hyy for WriteIMEI Protect for TY
#define FLAG_NV_DOWN                  (BIT_0)
#define FLAG_PS_DOWN                  (BIT_1)
#define FLAG_RES_DOWN                 (BIT_2)
#define FLAG_FLASH_DOWN               (BIT_3)
#define FLAG_FDL_DOWN                 (BIT_7)
#define FLAG_ALL_OK                   (FLAG_FDL_DOWN|FLAG_NV_DOWN|FLAG_PS_DOWN|FLAG_RES_DOWN|FLAG_FLASH_DOWN)
#define FIX_NV_IMEI_FLAG_OFFSET       (4)
#define FIX_NV_IMEI_FLAG_MAGIC        (0x13E1)// 1ME1

static uint8  erase_buf[M18_REGION_SIZE] = {0x0};

static NORMCP_ENTITY_PTR   s_normcp_entity_ptr    = PNULL;
FLASH_PARTITION_PTR    s_flash_partition_info_ptr = PNULL;
static FLASH_PARTITION_T  s_flash_partition_info  = {0};
static BOOLEAN           s_is_dload_udisk_img     = SCI_FALSE;

#ifdef FLASH_TYPE_SPI
static const SPIFLASH_SPEC_T *pSpiFlashSpec[SFC_MAX_CS] = {NULL};
uint8   nID_MID[SFC_MAX_CS]= {0}, nID_DID1[SFC_MAX_CS]= {0}, nID_DID2[SFC_MAX_CS]= {0};
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
static uint32 g_is_tfboot = 0;;

uint32 g_is_product_sector= 0;
BOOLEAN  g_is_dload_phasecheck = FALSE;

static BOOLEAN fdl_is_download_mode = SCI_FALSE;

uint32 g_prod_info_offset_addr = 0;
uint32 g_fdl_file_end_addr = 0;

#if defined(PLATFORM_UWS6121E)
//#define SRAM_BASE_ADDR          (PHY_RAM_BASE_ADDR) //move to common.h
//#define SRAM_START_ADDR         (PHY_RAM_BASE_ADDR) //move to common.h
#elif defined(PLATFORM_SC6531EFM)
#define SRAM_BASE_ADDR          (0x14000000)
#define SRAM_START_ADDR         (0x14100000)
#else
#define SRAM_BASE_ADDR          (0x34000000)
#define SRAM_START_ADDR         (0x34100000)
#endif
#define FIX_NV_LEN_OFFSET       (16)
#define PRD_TAIL_INFO_LEN       (8)
#define PRD_INFO_CRC_OFFSET     (6)
#define SECTOR_STATUS_OFFSET    (2)
#define PRD_STATUS_OFFSET       (SECTOR_STATUS_OFFSET)
//#define PROD_INFO_MIN_SIZE_4KB        (4096)
#define PROD_INFO_MIN_SIZE_512        (512)


#define FDL_INVALID_ADDR       0xFFFFFFFF
#define FDL_INVALID_SIZE       0xFFFFFFFF

#ifdef STONE_IMAGE_SUPPORT
#define   SINGLE_CS_MAX_FLASH_SIZE        (32*1024*1024)
#else
#define   SINGLE_CS_MAX_FLASH_SIZE        (0x1000000)
#endif

uint32 FDL_InEfsSpace (uint32 addr)
{
    return ( (addr >=s_flash_partition_info_ptr->EfsAdress) && (addr < s_flash_partition_info_ptr->EfsAdress + s_flash_partition_info_ptr->EfsSize));
}

uint32 FDL_EfsAddrConvert (uint32 addr)
{
    uint32 temp_addr;

    if ( (addr >=s_flash_partition_info_ptr->EfsAdress) && (addr < s_flash_partition_info_ptr->EfsAdress + s_flash_partition_info_ptr->EfsSize))
    {
        if (addr & 0x10)
        {
            temp_addr = 0x10;
            addr = (addr+ s_flash_partition_info_ptr->EfsSize) & (~temp_addr);
        }
    }

    return addr;
}







#if 0  //for uart cfg(debug)
#define OSI_ABS(type, a) ({ type _a = (type)(a); _a > 0? _a : -_a; })
#define UART1_SRC_FREQ (26000000)
#define UART4_SRC_FREQ (26000000)

typedef unsigned short uint16_t;
typedef unsigned long long uint64_t;

typedef struct
{
    uint16_t num;
    uint16_t denom;
} romFracDividerValue_t;

int builtin_clz(unsigned int x)
{
    int n = 0;
    x |=1;
    while(!(x & 0x80000000))
    {
        n++;
        x <<= 1;
    }
    return n;
}

romFracDividerValue_t romCalcFracDivider(unsigned input, unsigned output,
                                         unsigned num_bits,
                                         unsigned denom_bits)
{

    int delta = -1U;
    int rnum = 0, rdenom = 0;
    romFracDividerValue_t divider;
    int inavail_bits;
    unsigned int in_up=0;
    int num=0;
    unsigned int denom;
    unsigned int out_up;
    unsigned int diff;


    divider.denom = 0;
    divider.num = 0;
    inavail_bits = builtin_clz(input);

    if (input == 0 || output == 0 || output > input)
        return divider;

    // Ensure no overflow: input*num, output*denom

    if (inavail_bits < num_bits + 1)
    {
        input >>= (num_bits + 1 - inavail_bits);
        output >>= (num_bits + 1 - inavail_bits);
    }


    for (num = 1; num < (1 << num_bits); num++)
    {
         in_up = (unsigned int)input * num;
         denom = (in_up + output / 2) / output;
        if (denom >= (1 << denom_bits))
            break;

        // Maybe a little over-complex. If the difference is
        // calculated by (input*num/denom), sub-optimized value
        // may be choosen when output is small.
         out_up = (unsigned int)output * denom;
         diff = OSI_ABS(int, out_up - in_up);
        diff = ((uint64_t)diff << denom_bits) / denom;
        if (diff < delta)
        {
            delta = diff;
            rnum = num;
            rdenom = denom;
        }
        if (diff == 0)
            break;
    }

    if (delta == -1U)
        return divider;

    divider.num = rnum;
    divider.denom = rdenom;
    return divider;
}

void uart4_SetBuad(unsigned int Buad)
{
    REG_AP_APB_CFG_CLK_UART4_T AP_APB_CFG_CLK_UART4;
    romFracDividerValue_t divider = romCalcFracDivider(UART4_SRC_FREQ / 4, Buad, 13, 17);

    AP_APB_CFG_CLK_UART4.v = hwp_apApb->cfg_clk_uart4;
    AP_APB_CFG_CLK_UART4.b.uart4_div_num = divider.num;
    AP_APB_CFG_CLK_UART4.b.uart4_div_denom = divider.denom;
    hwp_apApb->cfg_clk_uart4 = AP_APB_CFG_CLK_UART4.v;
}

void uart1_SetBuad(unsigned int Buad)
{
    REG_LPS_APB_CFG_CLK_UART1_T LPS_APB_CFG_CLK_UART1;
    romFracDividerValue_t divider = romCalcFracDivider(UART1_SRC_FREQ / 4, Buad, 10, 14);

    LPS_APB_CFG_CLK_UART1.v = hwp_lpsApb->cfg_clk_uart1;
    LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_num = divider.num;
    LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_demod = divider.denom;
    LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_update = 1;
    hwp_lpsApb->cfg_clk_uart1 = LPS_APB_CFG_CLK_UART1.v;
}

void uart4_io_init(void)
{
    // UART4 is not default IO
    hwp_iomux_6121e->uart_2_cts = 0x10; // UART_4_RX
    hwp_iomux_6121e->uart_2_rts = 0x10; // UART_4_TX
    hwp_iomux_6121e->pad_uart_2_cts &= ~(IOMUX_PAD_UART_2_CTS_DRV(0x3));
    hwp_iomux_6121e->pad_uart_2_cts |= IOMUX_PAD_UART_2_CTS_DRV(2);
    hwp_iomux_6121e->pad_uart_2_cts &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_cts |= IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_rts &= ~(IOMUX_PAD_UART_2_RTS_DRV(0x3));
    hwp_iomux_6121e->pad_uart_2_rts |= IOMUX_PAD_UART_2_RTS_DRV(2);
}

void uart1_io_init(void)
{
    // UART1: set iomux
    hwp_iomux_6121e->pad_uart_1_rxd &= ~(IOMUX_PAD_UART_1_RXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_1_rxd |= IOMUX_PAD_UART_1_RXD_DRV(7); //rx_drving = 2
    hwp_iomux_6121e->pad_uart_1_rxd &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_1_rxd |= IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_1_txd &= ~(IOMUX_PAD_UART_1_TXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_1_txd |= IOMUX_PAD_UART_1_TXD_DRV(7); //rx_drving = 2
}

#endif






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

    rprintf("check_start_address: start 0x%x\n", start_addr);

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
        sio_trace ("some error");
    }
}


LOCAL void _FDL_SetMcuClock (void)
{

}

LOCAL void reset_mcu_clock (void)
{
    _FDL_SetMcuClock();
}

PUBLIC void FDL_SetDloaderMode (void)
{
    if (0x8A == FDL_GetBootMode()) //#define BOOT_FLAG_AUTODOWNLOAD (0x8A)
    {
        fdl_is_download_mode = SCI_TRUE;
    }
}

PUBLIC BOOLEAN FDL_IsDloaderMode (void)
{
    return fdl_is_download_mode;
}

//#ifdef SECURE_BOOT_SUPPORT
#if 0
volatile uint32 loop_var=0;

uint32 download_secure_check(uint8* secure_code_ptr, uint8* fdl_vlr_ptr)
{
#define CONFIG_LOAD_ADDRESS             (0x80008000)
#define FDL_PUBKEY_ADDR     (CONFIG_LOAD_ADDRESS + SECURE_CODE_OFF)
    uint8  *puuk_ptr               = (uint8*)(FDL_PUBKEY_ADDR);
    if(CheckSecureBootState())
    {
        return harshVerify((uint8 *)(secure_code_ptr), 0, \
            (uint8 *)fdl_vlr_ptr, (uint8 *)(puuk_ptr));
    }
    rprintf("SECURE_BOOT_SUPPORT disable, veriy by pass\n");
    return 1;
}

static BOOLEAN Check_Bootloader_Protect()
{
    if(CheckSecureBootState())
    {
        return TRUE;
    }
    return FALSE;
}

static void Set_Bootloader_HashLength(uint32 hash_length)
{
    //memcpy((char *)FLASH_START_ADRESS + HASH_OFF,&hash_length,sizeof(hash_length));
     if (FDL_FlashWriteArea (FLASH_START_ADRESS + HASH_OFF, (char *)&hash_length,sizeof(hash_length)) != sizeof(hash_length))
     {
        SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
        handle_error();
     }
}
#endif

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
#pragma arm section zidata = "NOR_FDL_STACK_SPACE"
uint32 stack_space[STACK_LIMIT>>2];
#pragma arm section zidata
const unsigned long stack_bottom = ( (unsigned long) stack_space + STACK_LIMIT);
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define PSRAM_START_ADDRESS 0x14000000
#else
#define PSRAM_START_ADDRESS 0x34000000
#endif
#pragma arm section code="CHIP_SETTING"
#if defined(CHIP_VER_UWS6121E)
void FDL_FlashSPIInit(int cs)
{
        Spiflash_ExtCfg_PRT flash_ext_cfg;

        //rprintf("FDL_FlashSPIInit: SFCDRV_CSCfg\n");
        SFCDRV_CSCfg (cs);

        //rprintf("FDL_FlashSPIInit: SFCDRV_ClkCfg\n");
        SFCDRV_ClkCfg(cs, (2<<4) | (8<<8));

        //rprintf("FDL_FlashSPIInit: SPIFLASH_ResetQPIFlash\n");
        SPIFLASH_ResetQPIFlash (cs);

        rprintf("FDL_FlashSPIInit: SPIFLASH_ReadID\n");
        SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

        if ( (nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);//no device
            rprintf("FDL_FlashSPIInit: BSL_UNKNOWN_DEVICE\n");
            while(1); //for debug
            return;
        }
        else
        {
            rprintf("cs%d ID:0x%x 0x%x 0x%x\n", cs, nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);
            FDL_SendTrace ("cs%d ID:0x%x 0x%x 0x%x", cs, nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);
        }

        /* set cs1 start address register before spiflash_cs1 initial */
        //rprintf("FDL_FlashSPIInit: SPIFLASH_SPEC_Get\n");
        pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);


        if (NULL ==  pSpiFlashSpec[cs])
        {
            rprintf("FDL_FlashSPIInit: NULL ==  pSpiFlashSpec\n");
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return;
        }

        //rprintf("FDL_FlashSPIInit: SPIFLASH_Init\n");
        SPIFLASH_Init (cs);

        prj_total_size += pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

        flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
        if (SPIFLASH_CS0 == cs)
        {
            flash_cs0_size = flash_ext_cfg->desity;
            //rprintf("FDL_FlashSPIInit: SPIFLASH_SetCS0DesityFlag\n");
            SPIFLASH_SetCS0DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);
        }
        else if (SPIFLASH_CS1 == cs)
        {
            flash_cs1_size = flash_ext_cfg->desity;
            //rprintf("FDL_FlashSPIInit: SPIFLASH_SetCS1DesityFlag\n");
            SPIFLASH_SetCS1DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs1_size);
        }
        rprintf("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x\n",
                   flash_cs0_size, flash_cs1_size, prj_total_size);
        FDL_SendTrace ("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x",
                   flash_cs0_size, flash_cs1_size, prj_total_size);
}
#endif



#if (defined(MULTI_FLASH_SUPPORT) || defined(FLASH_SIZE_128MBITX32MBIT))
static void prvFlashClkInit(void)
{
    REG_AP_CLK_CGM_SPIFLASH1_SEL_CFG_T CFG1;
    REG_AP_CLK_CGM_SPIFLASH2_SEL_CFG_T CFG2;
    REG_AP_APB_CLK_AP_EN0_T clken;
    REG_AP_APB_CLK_AP_EN1_T clken1;
    REG_AP_APB_CFG_CLK_SPIFLASH1_T ap_clk1;
    REG_AP_APB_CFG_CLK_SPIFLASH2_T ap_clk2;
    REG_SPI_FLASH_SPI_CONFIG_T SPI_CFG;

    hwp_iomux_6121e->gpio_18  =  0X01 << 2;  //SET FUNC= FLASH
    hwp_iomux_6121e->gpio_19  =  0X01 << 2;  //SET FUNC= FLASH
    hwp_iomux_6121e->gpio_20  =  0X01 << 2;  //SET FUNC= FLASH
    hwp_iomux_6121e->gpio_21  =  0X01 << 2;  //SET FUNC= FLASH
    hwp_iomux_6121e->gpio_22  =  0X01 << 2;  //SET FUNC= FLASH
    hwp_iomux_6121e->gpio_23  =  0X01 << 2;  //SET FUNC= FLASH

    hwp_iomux_6121e->pad_gpio_18 = 0x1fe000; //SET PULL/DRV
    hwp_iomux_6121e->pad_gpio_19 = 0x2fe000;
    hwp_iomux_6121e->pad_gpio_20 = 0x3fe000;
    hwp_iomux_6121e->pad_gpio_21 = 0x3fe000;
    hwp_iomux_6121e->pad_gpio_22 = 0x3fe000;
    hwp_iomux_6121e->pad_gpio_23 = 0x3fe000;

    ap_clk1.v = hwp_apApb->cfg_clk_spiflash1;
    ap_clk1.b.spiflash1_freq = 0xc;
    hwp_apApb->cfg_clk_spiflash1 = ap_clk1.v;

    ap_clk2.v = hwp_apApb->cfg_clk_spiflash2;
    ap_clk2.b.spiflash2_freq = 0xc;
    hwp_apApb->cfg_clk_spiflash2 = ap_clk2.v;

    clken.v = hwp_apApb->clk_ap_en0;
    clken.b.clk_en_spiflash1_fr = 1;
    clken.b.clk_en_spiflash2_fr = 1;
    hwp_apApb->clk_ap_en0 = clken.v;

    CFG1.v = hwp_apClk->cgm_spiflash1_sel_cfg;
    CFG1.b.cgm_spiflash1_sel = 0x4;
    hwp_apClk->cgm_spiflash1_sel_cfg = CFG1.v;

    CFG2.v = hwp_apClk->cgm_spiflash2_sel_cfg;
    CFG2.b.cgm_spiflash2_sel = 0x4;
    hwp_apClk->cgm_spiflash2_sel_cfg = CFG2.v;

    SPI_CFG.v = hwp_spiFlash->spi_config;
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    hwp_spiFlash->spi_config = SPI_CFG.v;

    SPI_CFG.v = hwp_spiFlashExt->spi_config;
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    hwp_spiFlashExt->spi_config = SPI_CFG.v;

    clken1.v = hwp_apApb->clk_ap_en1;
    clken1.b.clk_en_spiflash1 = 1;
    clken1.b.clk_en_spiflash2 = 1;
    hwp_apApb->clk_ap_en1 = clken1.v;

}

#else

static void prvFlashClkInit(void) //flash1 used only
{
    REG_AP_CLK_CGM_SPIFLASH1_SEL_CFG_T CFG1;
    REG_AP_APB_CLK_AP_EN0_T clken;
    REG_AP_APB_CFG_CLK_SPIFLASH1_T ap_clk1;
    REG_SPI_FLASH_SPI_CONFIG_T SPI_CFG;

    ap_clk1.v = hwp_apApb->cfg_clk_spiflash1;
    ap_clk1.b.spiflash1_freq = 0xc;
    hwp_apApb->cfg_clk_spiflash1 = ap_clk1.v;

    clken.v = hwp_apApb->clk_ap_en0;
    clken.b.clk_en_spiflash1_fr = 1;
    hwp_apApb->clk_ap_en0 = clken.v;

    CFG1.v = hwp_apClk->cgm_spiflash1_sel_cfg;
    CFG1.b.cgm_spiflash1_sel = 0x4;
    hwp_apClk->cgm_spiflash1_sel_cfg = CFG1.v;

    SPI_CFG.v = hwp_spiFlash->spi_config;
    SPI_CFG.b.clk_divider = 0x2;
    SPI_CFG.b.sample_delay = 0x2;
    SPI_CFG.b.quad_mode = 1;
    hwp_spiFlash->spi_config = SPI_CFG.v;
}


#endif



void _FDL_HWInit (void)
{
    uint32 i = 0;
    uint32 tmp = 0;
    uint32 crI = 0;
    uint32 crII = 0;

    //rprintf ("enter _FDL_HWInit!\n");

#ifdef PLATFORM_SC6530

    //rprintf ("enter _FDL_HWInit(PLATFORM_SC6530)!\n");
    if (!FDL_IsDloaderMode())
    {
        * (volatile uint32 *) 0x20000000 = 0x22220000;

        * (volatile uint32 *) 0x8B0000A0 = 0xFFFFFFDE;

#ifdef CHIP_VER_6531
        CHIP_PHY_MCU_CLK_Init();

        // SMC burst config
        {
            MMU_DisableIDCM();

            EMC_Init();

            MMU_EnableIDCM();
        }

#else
        i = REG32 (0x8b000040);
        i &= ~ 0x3;
        i |= 0x01;     //sel clk_mcu 208MHz
        REG32 (0x8b000040) = i;

        for (i = 0; i < 100; i++);

        // pSRAM burst config
        {
            MMU_DisableIDCM();

            CHIP_REG_SET (0x20000000, 0x222211E0);
            CHIP_REG_SET (0x20000004, 0x00008080);

            for (i = 0; i < 10; i++) {}

            // pSRAM burst config
            crI = 0x8191F;     // BCR: variable latency
            crII = 0x10;      // RCR

            // external memory config
            CHIP_REG_OR (0x20000024, BIT_17);  //CRE eb
            * (volatile uint16 *) (PSRAM_START_ADDRESS + (crI << 1)) = 0;

            for (i = 0; i < 10; i++) {}

            * (volatile uint16 *) (PSRAM_START_ADDRESS + (crII<<1)) = 0;

            for (i = 0; i < 10; i++) {}

            CHIP_REG_AND (0x20000024, ~BIT_17);

            for (i = 0; i < 10; i++) {}

            // EMC config
            CHIP_REG_SET (0x20000024, 0x00AC1FFF);
            CHIP_REG_SET (0x20000028, 0x015115FF);
            CHIP_REG_SET (0x2000002c, 0x00501015);

            CHIP_REG_SET (0x20000000, 0x222210E0);
            CHIP_REG_SET (0x20000004, 0x00008080);

            for (i = 0; i < 100; i++)
            {
                tmp = CHIP_REG_GET (PSRAM_START_ADDRESS);
            }

            MMU_EnableIDCM();
        }

#endif
    }

#ifdef FLASH_TYPE_SPI
    {
        Spiflash_ExtCfg_PRT flash_ext_cfg;
        int cs = 0;

        /* spiflash cs0 initial */
        * (volatile uint32 *) 0x20A00204 |= 3;
        * (volatile uint32 *) 0x20A0000C = 3; //clear sfc int
        cs = SPIFLASH_CS0;
        SFCDRV_CSCfg (cs);
        SPIFLASH_ResetQPIFlash (cs);

        SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

        if ( (nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);//no device
            return;
        }
        else
        {
            FDL_SendTrace ("cs0 ID:0x%x 0x%x 0x%x", nID_MID[0], nID_DID1[0], nID_DID2[0]);
        }

        /* set cs1 start address register before spiflash_cs1 initial */
        pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);


        if (NULL ==  pSpiFlashSpec[cs])
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return;
        }

        SPIFLASH_Init (cs);

        prj_total_size = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

        flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
        flash_cs0_size = flash_ext_cfg->desity;

        SFCDRV_SetCS1StartAddr (flash_cs0_size);
        SPIFLASH_SetCS0DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);


        // dual cs spiflash are used
        {
            /* spiflash cs1s initial */
            cs = SPIFLASH_CS1;
            SFCDRV_CSCfg (cs);
            SPIFLASH_ResetQPIFlash (cs);

            SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

            if ( (nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
            {
                //maybe cs1 has no device!
                return;
            }
            else
            {
                FDL_SendTrace ("cs1 ID:0x%x 0x%x 0x%x", nID_MID[1], nID_DID1[1], nID_DID2[1]);
            }

            pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);

            if (pSpiFlashSpec[cs] != NULL)
            {
                SPIFLASH_Init (cs);

                flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
                flash_cs1_size = flash_ext_cfg->desity;

                SPIFLASH_SetCS1DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs1_size);
            }
            else
            {
                SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
                return;
            }
        }

        FDL_SendTrace ("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x",
                       flash_cs0_size, flash_cs1_size, prj_total_size);

    }
#endif

#elif defined(PLATFORM_SC6531EFM)
    //rprintf ("_FDL_HWInit(PLATFORM_SC6531EFM)\n");
    * (volatile uint32 *) 0x8B0000A8 = 0xFFFFFFDE;
    {
        uint32 j;

        j = REG32 (APB_CLK_CFG3);

        //set ARM clk and AHB clk , don't gate.
        j &=  ~CLK_MCU_RATIO;
        REG32 (APB_CLK_CFG3) = j;

        //set CLK_ARM to 208MHz
        j &= ~ (CLK_MCU_SEL_MSK);
        j |= 0x3 << CLK_MCU_SEL_SHIFT;//0x0:26m, 0x1:104m_cpll, 0x2:208m_cpll, 0x3:208m_apll

        REG32 (APB_CLK_CFG3) = j;

        for (j=0; j<100; j++) {}

        //set SFC CLK to 104MHz
        j = REG32 (CGM_SFC_2X_CFG);
        j &=  ~CGM_SFC_2X_SEL_MSK;
        j |= 0x4 << CGM_SFC_2X_SEL_SHIFT;//0x4:208m_apll, 0x5:208m_cpll
        REG32 (CGM_SFC_2X_CFG) = j;

        j = REG32 (CGM_SFC_1X_CFG);
        j &=  ~CGM_SFC_1X_DIV_MSK;
        j |= 0x1 << CGM_SFC_1X_DIV_SHIFT;
        REG32 (CGM_SFC_1X_CFG) = j;

        for (j=0; j<100; j++) {}

    }

    //before set spiflash to QPI mode,enable software reset ldo sf power down
    ANA_REG_OR(ANA_SWRST_CTRL1, BIT_2);
    ANA_REG_OR(ANA_SWRST_CTRL0, (BIT_6|BIT_9|BIT_10));

#ifdef FLASH_TYPE_SPI
    {
        Spiflash_ExtCfg_PRT flash_ext_cfg;
        int cs = 0;

        /* spiflash cs0 initial */
        * (volatile uint32 *) 0x20A00204 |= 3;
        * (volatile uint32 *) 0x20A0000C = 3; //clear sfc int
        cs = SPIFLASH_CS0;
        SFCDRV_CSCfg (cs);
        SPIFLASH_ResetQPIFlash (cs);

        SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

        if ( (nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);//no device
            return;
        }
        else
        {
            FDL_SendTrace ("cs0 ID:0x%x 0x%x 0x%x", nID_MID[0], nID_DID1[0], nID_DID2[0]);
        }

        /* set cs1 start address register before spiflash_cs1 initial */
        pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);


        if (NULL ==  pSpiFlashSpec[cs])
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return;
        }

        SPIFLASH_Init (cs);

        prj_total_size = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

        flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
        flash_cs0_size = flash_ext_cfg->desity;

        SFCDRV_SetCS1StartAddr (flash_cs0_size);
        SPIFLASH_SetCS0DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);

        // dual cs spiflash are used
        #if 0
        {
            /* spiflash cs1s initial */
            cs = SPIFLASH_CS1;
            SFCDRV_CSCfg (cs);
            SPIFLASH_ResetQPIFlash (cs);

            SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

            if ( (nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
            {
                //maybe cs1 has no device!
                return;
            }
            else
            {
                FDL_SendTrace ("cs1 ID:0x%x 0x%x 0x%x", nID_MID[1], nID_DID1[1], nID_DID2[1]);
            }

            pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);

            if (pSpiFlashSpec[cs] != NULL)
            {
                SPIFLASH_Init (cs);

                flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
                flash_cs1_size = flash_ext_cfg->desity;

                SPIFLASH_SetCS1DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs1_size);
            }
            else
            {
                SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
                return;
            }
        }
        #endif

        FDL_SendTrace ("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x",
                       flash_cs0_size, flash_cs1_size, prj_total_size);

    }
#endif //FLASH_TYPE_SPI

#elif defined(PLATFORM_UWS6121E)


    rprintf ("_FDL_HWInit(PLATFORM_UWS6121E)\n");


#ifdef FLASH_TYPE_SPI
#if defined(CHIP_VER_UWS6121E)
    {
        int cs = 0;
        /* spiflash cs0 initial */
        cs = SPIFLASH_CS0;

        rprintf ("prvFlashClkInit!\n-----\n");
        prvFlashClkInit();

        //rprintf ("before FDL_FlashSPIInit!\n");
        FDL_FlashSPIInit(cs);
        //rprintf ("after FDL_FlashSPIInit!\n");

#ifndef SIMULATE_FLASH2_SUPPORT
        /* spiflash cs1 initial */
/*
        *(volatile unsigned int *)(0x5010c098) = 0x1;
        *(volatile unsigned int *)(0x5010c09C) = 0x1;
        *(volatile unsigned int *)(0x5010c0A0) = 0x1;
        *(volatile unsigned int *)(0x5010c0A4) = 0x1;
        *(volatile unsigned int *)(0x5010c0A8) = 0x1;
        *(volatile unsigned int *)(0x5010c0AC) = 0x1;
*/
        //cs = SPIFLASH_CS1;
        //rprintf ("before FDL_FlashSPIInit! SPIFLASH_CS1\n");  //debug flash2
        //FDL_FlashSPIInit(cs);
        //rprintf ("after FDL_FlashSPIInit!SPIFLASH_CS1\n");

#if defined(MULTI_FLASH_SUPPORT) || defined(FLASH_SIZE_128MBITX32MBIT)
        cs = SPIFLASH_CS1;
        rprintf ("before FDL_FlashSPIInit! SPIFLASH_CS1\n");
        FDL_FlashSPIInit(cs);
#endif

#endif
    }
#else
    {
        Spiflash_ExtCfg_PRT flash_ext_cfg;
        int cs = 0;

        /* spiflash cs0 initial */
        cs = SPIFLASH_CS0;
        SFCDRV_CSCfg (cs);
        SFCDRV_ClkCfg(cs, (2<<4) | (8<<8));

        SPIFLASH_ResetQPIFlash (cs);

        SPIFLASH_ReadID (cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

        if ( (nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);//no device
            return;
        }
        else
        {
            FDL_SendTrace ("cs0 ID:0x%x 0x%x 0x%x", nID_MID[0], nID_DID1[0], nID_DID2[0]);
        }

        /* set cs1 start address register before spiflash_cs1 initial */
        pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);


        if (NULL ==  pSpiFlashSpec[cs])
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return;
        }

        SPIFLASH_Init (cs);

        prj_total_size = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

        flash_ext_cfg = (Spiflash_ExtCfg_PRT) pSpiFlashSpec[cs]->pExtcfg;
        flash_cs0_size = flash_ext_cfg->desity;

        //SFCDRV_SetCS1StartAddr (flash_cs0_size);
        SPIFLASH_SetCS0DesityFlag (nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);

        FDL_SendTrace ("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x",
                       flash_cs0_size, flash_cs1_size, prj_total_size);
    }
#endif
#endif //FLASH_TYPE_SPI

#else // End PLATFORM_UWS6121E
    // hold the DSP before switch the share internal memory.
    * (volatile uint32 *)  GR_GEN0 |= 0x0400 ;

    REG32 (0x8b00000c) |= 0x03;     //sel clk_mcu 80MHZ

    for (i=0; i<100; i++);

    // EMC Timing Setting about asyc single mode
    REG32 (0x20000004) = 0x00000000;
    REG32 (0x20000000) = 0x11110000;

    //flash
    if (! ( (REG32 (0x20000014)) & BIT_23))
    {
        REG32 (0x20000014) = 0x00104FF0;
        REG32 (0x20000018) = 0x0151FAFA;
        REG32 (0x2000001C) = 0x00a06200;
        //sram
        REG32 (0x20000024) = 0x00104FF0;
        REG32 (0x20000028) = 0x0151FAFA;
        REG32 (0x2000002C) = 0x00a06200;
        //flash cs3
        REG32 (0x20000044) = 0x00104FF0;
        REG32 (0x20000048) = 0x0151FAFA;
        REG32 (0x2000004C) = 0x00a06200;

    }
    else
    {
        REG32 (0x20000014) = 0x00904FF0;
        REG32 (0x20000018) = 0x0151FAFA;
        REG32 (0x2000001C) = 0x0050622a;
        //sram
        REG32 (0x20000024) = 0x00904FF0;
        REG32 (0x20000028) = 0x0151FAFA;
        REG32 (0x2000002C) = 0x0050622a;
        //flash cs3
        REG32 (0x20000044) = 0x00904FF0;
        REG32 (0x20000048) = 0x0151FAFA;
        REG32 (0x2000004C) = 0x0050622a;
    }

    //_FDL_SetMcuClock();

    // enable system count
    REG32 (GR_GEN1) |= 0x2000;
#endif

    //rprintf ("END _FDL_HWInit!\n");

}
#pragma arm section code

#ifndef FLASH_TYPE_SPI
/************************************************************/
/********************** Hyy    ********************************/
//FDL也必须像大CODE一样上电后即进行SLAVE数据到有效区域FIXNV PROD等的恢复，这是第一步
//该函数用于FDL下载时的断电保护，如果手机在下载
//FDL或者正常运行时，处于擦出FIXNV或者PRODINFO的时候
//断电，但数据在SLAVE中，此时若是再次下载PROD或者FIXNV(注意是相反)
//则数据立即丢失，而且在FIXNV PRODINFO公用SECTOR的和其他数据公用的情况下，其他数据也丢失

//其根本原理上是和任何一次有效的启动都必须有相同的恢复机制，FDL与大CODE一致
//且此函数后，后面的逻辑会变得统一
/**************************************************************************************/
void update_sector_by_slave (void)
{
    uint32 slave_addr = 0xFFFFFFFF;
    uint32 slave_size = 0xFFFFFFFF;
    uint8 *slave_sec_buf = (uint8 *) SRAM_START_ADDR;
    NOR_FLASH_CONFIG_PTR flash_partition_info_in_ps = NULL;
    NV_STATUS_TYPE_E  slave_status = 0XFFFF;
    flash_partition_info_in_ps = get_flash_partition_info_InPS();

    //step1: 获取Slave Sector地址
    if (flash_partition_info_in_ps != NULL)
    {
        slave_addr = flash_partition_info_in_ps->efs_start_addr + FLASH_START_ADRESS ;
    }
    else
    {
        slave_addr = s_flash_partition_info_ptr->EfsAdress ;
    }

    //step2: 检查SLAVE中数据的类型和有效性
    slave_size= FDL_FlashCheckSectorSize (slave_addr);
    slave_status = FDL_FlashReadWord (slave_addr + slave_size - SECTOR_STATUS_OFFSET);

    if (slave_status == NV_SOK_PRD_INFO)
    {
        uint32 product_info_addr = 0XFFFFFFFF;
        uint32 product_info_sec_addr = 0XFFFFFFFF;
        uint32 product_info_sec_size = 0;
        uint16 prod_sec_dedicate_crc = 0;
        uint16 prod_sec_figure_crc =1;
        //此处两地址只要是目标地址即可。
        product_info_addr = s_flash_partition_info_ptr->ProdinfoAdress;
        product_info_sec_size = FDL_FlashCheckSectorSize (product_info_addr);
        product_info_sec_addr = (product_info_addr/product_info_sec_size) * product_info_sec_size;
        //@为了支持M18
        FDL_FlashRead (slave_addr, slave_sec_buf, slave_size);

        //@计算PRODUCT INFO的CRC与上次下载的记录值比较
        //PRODUCT INFO的CRC的位置记录在SLAVE SIZE的后6BYTE，注意不是PRODUCT SIZE的，因为可能不同
        prod_sec_dedicate_crc = * (uint16 *) &slave_sec_buf[slave_size -PRD_INFO_CRC_OFFSET];
        //CRC的计算区是SLAVE SIZE的开头到后8BYTE
        prod_sec_figure_crc     = crc16 (0, slave_sec_buf, slave_size -PRD_TAIL_INFO_LEN);

        /*如果SLAVE SEC区数据是好的*/
        if ( (prod_sec_dedicate_crc == prod_sec_figure_crc) ||
                (prod_sec_dedicate_crc == EndianConv_16 (prod_sec_figure_crc)))
        {
            //该函数有擦
            //把Slave数据Copy到Prod Sector中，该情况下不需要考虑各种公用的情况，只需要把数据拿到合适的位置，还需要下载

            FDL_FlashWriteArea (product_info_sec_addr, (char *) slave_sec_buf,  product_info_sec_size);
        }
    }
    else if (slave_status == NV_MINVALID_SOK)
    {
        uint32 slave_fixnv_length = FDL_FlashReadWord (slave_addr + slave_size - FIX_NV_LEN_OFFSET);
        uint16 slave_fixnv_dedicate_crc = 0;
        uint16 slave_fixnv_figure_crc =1;

        if (slave_fixnv_length > (slave_size - FIX_NV_LEN_OFFSET))
        {
            return;
        }

        //@为了支持M18
        FDL_FlashRead (slave_addr, slave_sec_buf, slave_size);
        //@计算FIXNV的CRC与上次下载时记录的CRC比较
        slave_fixnv_dedicate_crc = * (uint16 *) &slave_sec_buf[0];
        slave_fixnv_figure_crc = crc16 (0, slave_sec_buf + sizeof (uint16), slave_fixnv_length -sizeof (uint16));

        /*加FIXNV STATUS状态主要是用来区别该SECTOR在擦除过程中部分时间段
        区域出现全零情况,在全零情况下crc无法检测出问题*/
        if ( (slave_fixnv_dedicate_crc == slave_fixnv_figure_crc) ||
                (slave_fixnv_dedicate_crc == EndianConv_16 (slave_fixnv_figure_crc)))
        {
            uint32 fix_nv_size = 0;

            //@step3:增加对FIXNV结构的检查，思想是确保每次往FIXNV写都是有效的写。后面也一样，需要每次对FIXNV区域的写都是有效的，剩下的只可能是FLASH问题
            //@获得记录再SLAVE中FIXNV的SIZE与结构扫描下来的SIZE进行比对，相同认为是最终有效的FIXNV，且完成了对 FIXNV结构的检查
            fix_nv_size = FDL_FlashReadWord (slave_addr + slave_size - FIX_NV_LEN_OFFSET);

            if (fix_nv_size != calculate_fixNV_length (slave_addr, slave_size+slave_addr))
            {
                return;
            }

            * (uint16 *) &slave_sec_buf[slave_size - SECTOR_STATUS_OFFSET] = NV_MOK_SINVALID;
            * (uint16 *) &slave_sec_buf[slave_size - FIX_NV_LEN_OFFSET] = slave_fixnv_length;
            FDL_FlashWriteArea (s_flash_partition_info_ptr->NvAdress, (char *) slave_sec_buf, slave_size);
        }
    }

    //保留SLAVE不擦，可认为以上只是做一些防数据丢失的准备。另外寻找FIXNV的逻辑需要用到SLAVE，那个逻辑更完备，仍需要走。
}
#endif

void FDL_SendVerStr (void)
{
    PACKET_T *packet_ptr = PNULL;

    packet_ptr = FDL_MallocPacket();

    packet_ptr->packet_body.type = BSL_REP_VER;
    packet_ptr->packet_body.size = sizeof (version_string);
    FDL_memcpy (packet_ptr->packet_body.content, version_string, sizeof (version_string));

    FDL_SendPacket (packet_ptr);

    FDL_FreePacket (packet_ptr);

    {
        int i;

        for (i=0; i<2000; i++);
    }
}

void FDL_SendFlashID (void)
{
    PACKET_T *packet_ptr = PNULL;
    char str_buf[128] = {0};

    packet_ptr = FDL_MallocPacket();

    packet_ptr->packet_body.type = BSL_REP_VER;
    packet_ptr->packet_body.size = sizeof (str_buf);
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    sprintf (str_buf, "nID_MID[0]=0x%x, nID_DID1[0]=0x%x, nID_DID2[0]=0x%x",nID_MID[0], nID_DID1[0], nID_DID2[0]);
#else
    sprintf (str_buf, "nID_MID[0]=0x%x, nID_DID1[0]=0x%x, nID_DID2[0]=0x%x, \
    nID_MID[1]=0x%x, nID_DID1[1]=0x%x, nID_DID2[1]=0x%x",
    nID_MID[0], nID_DID1[0], nID_DID2[0], nID_MID[1], nID_DID1[1], nID_DID2[1]);
#endif

    FDL_memcpy (packet_ptr->packet_body.content, str_buf, sizeof (str_buf));

    FDL_SendPacket (packet_ptr);

    FDL_FreePacket (packet_ptr);

    {
        int i;

        for (i=0; i<2000; i++);
    }

}


#ifdef UMEM_SUPPORT
LOCAL uint32 FLASH_GetUmemVirSize (UMEM_TYPE_E index)
{
    uint32 umem_size = 0;
    uint32 vir_sec_size = 512;

#ifndef NOR_HIDDEN_UDISK_SUPPORT
    umem_size = s_flash_partition_info_ptr->UmemSize;

    if (s_intel_m18_serial)
    {
        if (umem_size > 4*1024*1024)
        {
            SCI_ASSERT (0); /*assert to do*/
        }
        else
        {
            vir_sec_size = 1024;
        }
    }
    else
    {
        if (umem_size<=0x200000)
        {
            vir_sec_size  =  512;
        }
        else if ( (umem_size>0x200000) && (umem_size<=0x400000))
        {
            vir_sec_size  =  1024;
        }
        else if ( (umem_size>0x400000) && (umem_size<=0x800000))
        {
            vir_sec_size  =  2048;
        }
        else if ( (umem_size>0x800000) && (umem_size<=0x1000000))
        {
            vir_sec_size  =  4096;
        }
        else
        {
            SCI_ASSERT (0); /*assert to do*/
        }
    }

#else

    if (index == UMEM_TYPE_FS)
    {
        umem_size = s_flash_partition_info_ptr->FSSize;
    }
    else if (index == UMEM_TYPE_HFS)
    {
        umem_size = s_flash_partition_info_ptr->HFSSize;
    }

    if (s_intel_m18_serial)
    {
        if (umem_size > 4*1024*1024)
        {
            SCI_ASSERT (0); /*assert to do*/
        }
        else
        {
            vir_sec_size = 1024;
        }
    }
    else
    {
        if (umem_size<=0x200000)
        {
            vir_sec_size  =  512;
        }
        else if ( (umem_size>0x200000) && (umem_size<=0x400000))
        {
            vir_sec_size  =  1024;
        }
        else if ( (umem_size>0x400000) && (umem_size<=0x800000))
        {
            vir_sec_size  =  2048;
        }
        else if ( (umem_size>0x800000) && (umem_size<=0x1000000))
        {
            vir_sec_size  =  4096;
        }
        else
        {
            SCI_ASSERT (0); /*assert to do*/
        }
    }

#endif
    return vir_sec_size;
}

#ifdef UMEM_NEW_MECHNISM
LOCAL uint16 FDL_GetIDBase (UMEM_TYPE_E index)
{
    if (index == UMEM_TYPE_FS)
    {
        return NV_UDISK_BASE;
    }
    else if (index == UMEM_TYPE_HFS)
    {
        return NV_UDISK_BASE + (s_flash_partition_info_ptr->UmemSize/FLASH_GetUmemVirSize (index));
    }
    else
    {
        return 0xffff;
    }
}


LOCAL void FDL_EfsEraseStatusSet (uint32 base)
{
    uint32 sec_size = (FDL_FlashCheckSectorSize (base));
    uint32 sec_addr = (base/sec_size) *sec_size;
    uint16 erase_status = 0x0;
    erase_status = FDL_FlashReadWord (sec_addr+sec_size-2);

    if (erase_status != 0x55aa)
    {
        if (erase_status != 0xffff)
        {
            FDL_FlashErase (sec_addr, sec_size);
        }

        SPIFLASH_WriteWord (sec_addr+sec_size-2, 0x55aa);
    }
}

LOCAL int flash_data_check (uint32 flash_base, uint32 base,uint32 len)
{
    uint32 i =0;
    uint32 ok_cnt = 0;
    uint32 u16_size = len >> 1;
    for(i = 0; i < u16_size; i++)
    {
        if((*(uint16 *)(base)) == (*(volatile uint16 *)(flash_base)))
            ok_cnt++;
        else
        {
             rprintf ("flash_data_check:addr 0x%x/0x%x, flash addr 0x%x/0x%x\n",base,(*(uint16 *)(base)),flash_base,(*(volatile uint16 *)(flash_base)));
        }

        flash_base      += 2;
        base         += 2;
    }

    if(ok_cnt != u16_size)
        return -1;
    else
        return 0;
}
 uint32  try_cnt = 5;

#define  ITEM_HEAD_SIZE      8
#define  VIR_SEC_SIZE_MAX  4096
LOCAL  uint8   item[ITEM_HEAD_SIZE+VIR_SEC_SIZE_MAX] = {0};
LOCAL  uint16  start_ind[UMEM_TYPE_MAX]= {0, 0};
LOCAL  uint32  addrbase[UMEM_TYPE_MAX]= {0,0};
LOCAL void FDL_Udisk_Img_Gen (UMEM_TYPE_E index, uint32 base,  char *buf, uint32 len)
{

    uint16  i;
    uint16 item_head[4];
    uint32 vir_sec_size  =  FLASH_GetUmemVirSize (index);
    uint32 vir_sec_num   = (len+vir_sec_size+ITEM_HEAD_SIZE-1) / (vir_sec_size+ITEM_HEAD_SIZE);
    uint32 write_size    =  0;
    uint32 full_size     =  len;
    uint32 align_size = 0;

    //FDL_SendAckPacket(0xaa);
    FDL_memset ( (uint8 *) &item[0], 0xff, (ITEM_HEAD_SIZE+vir_sec_size));

    if (addrbase[index] == 0x0)
    {
        addrbase[index] = base;
    }

    for (i=0x0; i<vir_sec_num; start_ind[index]++,i++)
    {
        uint32 sec_size = FDL_FlashCheckSectorSize (addrbase[index]);
        write_size = (full_size<vir_sec_size) ?full_size:vir_sec_size;

    //over block
        if ( ( (addrbase[index]+ITEM_HEAD_SIZE+vir_sec_size) /sec_size) != (addrbase[index]/sec_size))
        {
            //addrbase[index] += sec_size;
        align_size = addrbase[index] % sec_size;
        if ( align_size!= 0)
            {
                addrbase[index] += (sec_size-align_size);
            }
        }

    //data ok
        FDL_memcpy ( (uint8 *) &item[ITEM_HEAD_SIZE], (uint8 *) (buf+i*vir_sec_size), write_size);

    //header
     item_head[0]= (FDL_GetIDBase (index) + start_ind[index]);
        item_head[2]= item_head[0];
        item_head[3]= vir_sec_size;

        FDL_memcpy ( (uint8 *) &item[4], (uint8 *) &item_head[2], ITEM_HEAD_SIZE-4);

    //write to flash
        if (FDL_FlashWriteArea (addrbase[index]+4, (char *) &item[4], (ITEM_HEAD_SIZE+write_size-4)) != (ITEM_HEAD_SIZE+write_size-4))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            handle_error();
        }

    //try_cnt = 0xFFFFFFFF;
    try_cnt = 1;
    while(try_cnt)
    {
        if(0 == flash_data_check(addrbase[index]+4,(uint32)&item[4],(ITEM_HEAD_SIZE+write_size-4)))
            break;

        try_cnt++;
    }

     rprintf ("FDL_Udisk_Img_Gen:check size 0x%x, addr 0x%x, %d counts\n",(ITEM_HEAD_SIZE+write_size-4),addrbase[index]+4,try_cnt);

    //checksum
        item_head[1]= flash_get_checksum ( (uint32) (addrbase[index]+4), (uint16) ( (vir_sec_size + 4 + 1) >> 1));
        FDL_FlashWriteWord (addrbase[index],   item_head[0]);
        FDL_FlashWriteWord (addrbase[index]+2, item_head[1]);

    //try_cnt = 0xFFFFFFFF;
    #if 0
    try_cnt= 1;
    while(try_cnt)
    {
        if(0 == flash_data_check(addrbase[index],(uint32)&item_head[0],4))
            break;

        try_cnt++;
    }
     rprintf ("FDL_Udisk_Img_Gen:check size 4, addr 0x%x, %d counts\n",addrbase[index],try_cnt);
    #endif

        rprintf ("\nFDL_Udisk_Img_Gen:addr 0x%x, %x %x %x %x\n",addrbase[index], item_head[0], item_head[1],item_head[2],item_head[3]);


        FDL_EfsEraseStatusSet (addrbase[index]);
        addrbase[index] += (ITEM_HEAD_SIZE+vir_sec_size);

        if (full_size >= vir_sec_size)
        {
            full_size -= vir_sec_size;
        }
    }
}
#endif
#endif
#if !defined(PLATFORM_UWS6121E)
extern volatile int rx_buf_rindex;
extern volatile int rx_buf_actual;
#endif

typedef enum uart_instance
{
    uart1_instance = 1,
    uart2_instance,
    uart3_instance,
    uart4_instance,
}uart_instance_t;


int main (void)
{
    char ch;
    uint32 flash_base = 0;

    ESAL_TS_RTE_CP_WRITE(p15, 0, 0x80100600, c12, c0, 0); //配置向量表起始地址
    ESAL_TS_RTE_CP_WRITE(p15, 0, 0x80100600, c12, c0, 1);

    serial_init(uart2_instance);

    rprintf ("\nfdl2 start running! %s %d\n", __func__, __LINE__);
#if !defined(PLATFORM_UWS6121E)
    rx_buf_rindex = 0;
    rx_buf_actual = 0;
#endif
    MMU_Init();

    FDL_SetDloaderMode();

    if (FDL_IsDloaderMode())
    {
    #if defined(PLATFORM_UWS6121E)
        usb_boot (1);  //autodownload mode,init usb_boot
    #endif
    }

#if defined(PLATFORM_SC6531EFM)
    ADI_init();
#endif

    //rprintf ("\nbefore FDL_memset!\n");
    //Initialize the global variable
    FDL_memset (&global_sys_stage, 0, sizeof (global_sys_stage));

    //rprintf ("\nbefore FDL_PacketInit!\n");
    FDL_PacketInit();

    //rprintf ("\nbefore _FDL_HWInit!\n");
    _FDL_HWInit();

    //rprintf ("\nbefore FDL_DlInit!\n");
    FDL_DlInit();

#if !defined(PLATFORM_SC6531EFM) //&& !defined(PLATFORM_UWS6121E)
    FDL_DlReg (BSL_CMD_CONNECT,    FDL_SysConnect,  0);
#endif

#ifdef SECURE_BOOT_SUPPORT
              bootGetEfuseCtx();
#endif
    FDL_DlReg (BSL_CMD_START_DATA, FDL_DataStart,   0);
    FDL_DlReg (BSL_CMD_MIDST_DATA, FDL_DataMidst,   0);
    FDL_DlReg (BSL_CMD_END_DATA,   FDL_DataEnd,     0);
    FDL_DlReg (BSL_CMD_EXEC_DATA,  FDL_DataExec,    0);
    FDL_DlReg (BSL_SET_BAUDRATE,   FDL_SetBaudrate, 0);
    FDL_DlReg (BSL_CMD_NORMAL_RESET,   FDL_McuResetNormal, 0);
    FDL_DlReg (BSL_CMD_READ_FLASH,   FDL_McuReadFlash, 0);
    FDL_DlReg (BSL_CMD_READ_CHIP_TYPE,   FDL_McuReadChipType, 0);
    FDL_DlReg (BSL_CMD_LOOKUP_NVITEM,   FDL_LookupNvitem, 0);
    FDL_DlReg (BSL_ERASE_FLASH,   FDL_EraseFlash, 0);
    FDL_DlReg (BSL_CMD_POWER_OFF,   FDL_PowerOff, 0);

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)

    //rprintf ("\nbefore FDL_ChannelGet!\n");
    gFdlUsedChannel = FDL_ChannelGet();
    while (1)
    {
        ch = gFdlUsedChannel->GetChar (gFdlUsedChannel);

        if (0x7e == ch)
        {
            break;
        }
    }

    //rprintf ("\nbefore FDL_SendVerStr!\n");
    FDL_SendVerStr();
    //FDL_SendFlashID();
#endif

    // send true adress of nv,ps,res...to pc
    s_flash_partition_info_ptr = &s_flash_partition_info;

   if( !get_flash_partition_info (s_flash_partition_info_ptr))
        rprintf ("\nget_flash_partition_info failure\n");

#if  defined(MEM_STRESS_TEST_NORFLASH)
    flash_base = get_flash_base_addr(s_flash_partition_info_ptr->NorBootloaderAdress);
    norflash_test(flash_base,flash_base+s_flash_partition_info_ptr->FlashSize);
    return;
#endif

#if defined(PLATFORM_UWS6121E)
    rprintf ("\nNorBootloader: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->NorBootloaderAdress,
        s_flash_partition_info_ptr->NorBootloaderSize);
 #ifndef FOTA_SUPPORT_NONE
    rprintf ("\nFotaBootloader: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->FotaBootloaderAdress,
        s_flash_partition_info_ptr->FotaBootloaderSize);
 #endif
  #ifndef FOTA_SUPPORT_NONE
    rprintf ("\nFotadata: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->FotaAdress,
        s_flash_partition_info_ptr->FotaSize);
 #endif
    rprintf ("PsAdress: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->PsAdress, s_flash_partition_info_ptr->PSSize);
    rprintf ("ResAdress: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->ResAdress, s_flash_partition_info_ptr->ResSize);
    rprintf ("UmemAdress: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->UmemAdress, s_flash_partition_info_ptr->UmemSize);
    rprintf ("NvAdress: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->NvAdress, s_flash_partition_info_ptr->NvSize);
    rprintf ("ProdinfoAdress: 0x%x\n",
        s_flash_partition_info_ptr->ProdinfoAdress);
    rprintf ("EfsAdress: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->EfsAdress, s_flash_partition_info_ptr->EfsSize);
    rprintf ("Spload_Addr: 0x%x, 0x%x\n\n",
        s_flash_partition_info_ptr->Spload_Addr, s_flash_partition_info_ptr->SploadSize);
    rprintf ("DeltaNv_Addr: 0x%x, 0x%x\n\n",
        s_flash_partition_info_ptr->OperatorDataAdress, s_flash_partition_info_ptr->OperatorDataSize);
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    rprintf ("APN_Addr: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->apn_addr, s_flash_partition_info_ptr->apn_size);
#endif
    rprintf ("ConfigNV_Addr: 0x%x, 0x%x\n",
        s_flash_partition_info_ptr->config_nv_addr, s_flash_partition_info_ptr->config_nv_size);
#endif

#ifdef FLASH_TYPE_SPI
    initialize_nv_status (pSpiFlashSpec[0]->spiflash_cfg, FLASH_START_ADRESS);
#else
    initialize_nv_status (s_normcp_entity_ptr->nor_flash_cfg_ptr,FLASH_START_ADRESS);
#endif

    //update_sector_by_slave();

    g_write_imei_protect_flag = FLAG_FDL_DOWN;

    if (CHIP_PHY_Get7sRstFlag())
    {
        CHIP_PHY_Clean7sRstFlag();
    }

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)

    /* Reply the EXEC cmd received in the 1st FDL. */
    FDL_SendAckPacket (BSL_REP_ACK);

    /* Start the download process. */
    FDL_DlEntry(DL_STAGE_CONNECTED);

#else

    FDL_DlEntry (0);

#endif

    return 1;
}


int FDL_SysConnect (PACKET_T *packet_ptr , void *arg)
{
    if (SYS_STAGE_CONNECTED != global_sys_stage.data.stage)
    {
        FDL_SendAckPacket (BSL_REP_ACK);
        global_sys_stage.data.stage = SYS_STAGE_CONNECTED;
    }

    return 0;
}
uint32 NV_Size_Test;
int FDL_DataStart (PACKET_T *packet_ptr , void *arg)
{
    unsigned long  start_addr;
    unsigned long  file_size;
    unsigned long *ptr;
    unsigned long i;
    g_is_dload_phasecheck = FALSE;

    /*if ( (global_sys_stage.data .stage != SYS_STAGE_CONNECTED) &&
            (global_sys_stage.data .stage != SYS_STAGE_END))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_MULTI_START);
        return 0;
    }*/

#ifdef FLASH_TYPE_SPI
    {
        uint32 cs = SPIFLASH_CS0;

        if (NULL == pSpiFlashSpec[cs])
        {
            //unknown device
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
        }

        #ifdef PLATFORM_SC6530
        cs = SPIFLASH_CS1;

        if ( (Is_FlashID_Valid (nID_MID[cs], nID_DID1[cs], nID_DID2[cs])) &&
                (NULL == pSpiFlashSpec[cs]))
        {
            //unknown device
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
        }
        #endif

        //rprintf("FDL_DataStart prj_total_size[0x%x]",prj_total_size);
        //rprintf("FDL_DataStart flash_cs0_size[0x%x]",flash_cs0_size);
        //rprintf("FDL_DataStart flash_cs1_size[0x%x]",flash_cs1_size);

        if (prj_total_size > (flash_cs0_size + flash_cs1_size))
        {
            //total size error.
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
        }
    }
#endif

    ptr = (unsigned long *) packet_ptr->packet_body.content;

    /* analyse the packet*/
    start_addr     = *ptr++;
    start_addr =  EndianConv_32 (start_addr);
    file_size      = *ptr++;
    file_size  = EndianConv_32 (file_size);

    rprintf("FDL_DataStart 0x%x, %u\n", start_addr, file_size);

    s_is_dload_udisk_img = SCI_FALSE;

    if (packet_ptr->packet_body.size == 12)
    {
        g_checksum = *ptr;
        g_checksum = EndianConv_32 (g_checksum);
        g_sram_addr = SRAM_START_ADDR;
    }
    else
    {
        g_checksum = CHECKSUM_OTHER_DATA;
    }

    if (0 == g_checksum & 0xffffff)
    {
        //The fixnv checksum is error.
        SEND_ERROR_RSP (BSL_EEROR_CHECKSUM);
    }

    if (TFBOOT_LOGIC_LOAD_ADRESS == start_addr)
    {
        g_is_tfboot = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
        return 0;
    }


    if ( (NV_LOGIC_ADRESS == start_addr) || (NV_ALL_LOGIC_ADRESS == start_addr) || (PS_LOGIC_ADRESS ==  start_addr)
            || (RES_LOGIC_ADRESS == start_addr) || (PHASECHECK_LOGIC_ADRESS ==  start_addr)
            || (UMEM_LOGIC_ERASE_ADRESS == start_addr) || (UMEM_LOGIC_LOAD_ADRESS == start_addr)
            || (NOR_BOOTLOADER_LOGIC_ADRESS == start_addr)
            || (CP_CODE_LOGIC_LOAD_ADRESS == start_addr)
         #ifdef DSP_DOWNLOAD_SUPPORT
            || (DSP_LOGIC_LOAD_ADRESS == start_addr)
         #endif
         #if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
            || (FOTA_BOOTLOADER_LOGIC_ADRESS == start_addr)
            || (FOTA_PACKAGE_LOGIC_ADDRESS == start_addr)
         #endif
            )
    {
        //s_flash_partition_info_ptr = &s_flash_partition_info;

        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return 0;
        }
    }


    if (NV_LOGIC_ADRESS == start_addr)
    {
        start_addr = s_flash_partition_info_ptr->NvAdress;

        if (file_size > s_flash_partition_info_ptr->NvSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }

        for (i=0; i < (uint32) s_flash_partition_info_ptr->NvSize; i++)
        {
            * (uint8 *) (g_sram_addr+i) = 0xFF;
        }
    }


    if (NV_ALL_LOGIC_ADRESS == start_addr)
    {
        g_sram_addr = SRAM_START_ADDR;
        start_addr = s_flash_partition_info_ptr->NvAdress;

        if (file_size > (s_flash_partition_info_ptr->NvSize + PRODUCT_NV_SZE))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }

        for (i=0; i < (uint32)(s_flash_partition_info_ptr->NvSize + PRODUCT_NV_SZE); i++)
        {
            * (uint8 *) (g_sram_addr+i) = 0xFF;
        }
    }

#ifdef NOR_BOOTLOADER_SUPPORT
    else if (NOR_BOOTLOADER_LOGIC_ADRESS == start_addr)
    {
        start_addr = s_flash_partition_info_ptr->NorBootloaderAdress;

        if (file_size > s_flash_partition_info_ptr->NorBootloaderSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }
    }
    else if (DELTA_NV_LOGIC_LOAD_ADRESS == start_addr)
    {
         //start_addr = 0x60f00000;
         //file_size = 16 * 1024;
        start_addr = s_flash_partition_info_ptr->OperatorDataAdress;

        if (file_size > s_flash_partition_info_ptr->OperatorDataSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);

            return 0;
        }
    }
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    else if (APN_LOGIC_LOAD_ADRESS == start_addr)
    {
        start_addr = s_flash_partition_info_ptr->apn_addr;
        if (file_size > s_flash_partition_info_ptr->apn_size)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);

            return 0;
        }
    }
#endif
#endif
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    else if (FOTA_BOOTLOADER_LOGIC_ADRESS == start_addr)
        {
        start_addr = s_flash_partition_info_ptr->FotaBootloaderAdress;

            if (file_size > s_flash_partition_info_ptr->FotaBootloaderSize)
            {
                    SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
                    return 0;
            }
        }
    else if (FOTA_PACKAGE_LOGIC_ADDRESS == start_addr)
        {
        start_addr = s_flash_partition_info_ptr->FotaAdress;

            if (file_size > s_flash_partition_info_ptr->FotaSize)
            {
                    SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
                    return 0;
            }
        }
#endif
    else if (PS_LOGIC_ADRESS ==  start_addr)
    {
        start_addr = s_flash_partition_info_ptr->PsAdress;

        if (file_size > s_flash_partition_info_ptr->PSSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);

            return 0;
        }

        #ifdef DSP_DOWNLOAD_SUPPORT
        if (s_flash_partition_info_ptr->DspCodeAddress > s_flash_partition_info_ptr->PsAdress)
        {
            FDL_FlashErase (s_flash_partition_info_ptr->DspCodeAddress, s_flash_partition_info_ptr->DspCodeSize);
        }
        #endif //DSP_DOWNLOAD_SUPPORT
    }
    else if (RES_LOGIC_ADRESS == start_addr)
    {
        start_addr = s_flash_partition_info_ptr->ResAdress;

        if (file_size > s_flash_partition_info_ptr->ResSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }
    }
    else if (PHASECHECK_LOGIC_ADRESS ==  start_addr)
    {
        g_is_dload_phasecheck = TRUE;
        start_addr = s_flash_partition_info_ptr->ProdinfoAdress;
    }
    else if (UMEM_LOGIC_LOAD_ADRESS == start_addr)
    {
        s_is_dload_udisk_img = TRUE;
        g_umem_image_size = 0x0;
#ifdef UMEM_CTRL_SEC_EN
        start_addr = FLASH_START_ADRESS +  s_flash_partition_info_ptr->UmemAdress \
                     + FDL_FlashCheckSectorSize (FLASH_START_ADRESS+s_flash_partition_info_ptr->UmemAdress);
#else
        start_addr = FLASH_START_ADRESS +  s_flash_partition_info_ptr->UmemAdress;
#endif

        if (file_size > s_flash_partition_info_ptr->UmemSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }
    }
    else if (SPLOAD_LOGIC_LOAD_ADRESS  ==  start_addr)
    {
        start_addr = s_flash_partition_info_ptr->Spload_Addr;

        if (file_size > s_flash_partition_info_ptr->SploadSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }
    }
    else if (CP_CODE_LOGIC_LOAD_ADRESS  ==  start_addr)
    {
        start_addr = s_flash_partition_info_ptr->CpCodeAddress;

        if (file_size > s_flash_partition_info_ptr->CpCodeSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }
    }
    #ifdef DSP_DOWNLOAD_SUPPORT
    else if (DSP_LOGIC_LOAD_ADRESS  ==  start_addr)
    {
        start_addr = s_flash_partition_info_ptr->DspCodeAddress;

        if (file_size > s_flash_partition_info_ptr->DspCodeSize)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return 0;
        }
    }
    #endif


    if (!check_start_address (start_addr))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
        return 0;
    }

    if (!check_access_size (start_addr, file_size))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
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
    FDL_FlashWriteEnable (get_flash_base_addr (start_addr), 1);

    // send a ack immediatelly to allow PC to send next packet.
    if (!packet_ptr->ack_flag)
    {
        packet_ptr->ack_flag = 1;

        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 0;
}
int FDL_DataMidst (PACKET_T *packet_ptr, void *arg)
{
    unsigned short  data_len;
    uint32 j, sector_size, slave_size;
    uint32 cur_addr, partition_addr;
    BOOLEAN udisk_new_sceme = SCI_FALSE;

    /*if ( (global_sys_stage.data .stage != SYS_STAGE_START) &&
            (global_sys_stage.data .stage != SYS_STAGE_GATHER))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_NOT_START);
        return 0;
    }*/

    if (g_is_tfboot == 1)
    {
        FDL_SendAckPacket (BSL_REP_ACK);
        return 0;
    }

    // analyse the packet
    data_len     = packet_ptr->packet_body.size; //((pkt_header_ptr)pkt_buf)->pkt_size;
    //rprintf("FDL_DataMidst: %d\n", data_len);

    if ( (global_sys_stage.data .recv_size + data_len) > global_sys_stage.data .file_size)
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    // send a ack immediatelly to allow PC to send next packet.
    if ( (!packet_ptr->ack_flag) && (s_intel_m18_serial == 0) && (g_is_dload_phasecheck == FALSE))
    {
        packet_ptr->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }


    if (CHECKSUM_OTHER_DATA == g_checksum)
    {

#ifdef FLASH_TYPE_SPI
//#ifdef SECURE_BOOT_SUPPORT
#if 0
#if !defined(PLATFORM_UWS6121E)
    if(s_flash_partition_info_ptr->NorBootloaderAdress == global_sys_stage.data.start_addr)
    {
        if(Check_Bootloader_Protect())
        {
            if (!packet_ptr->ack_flag)
            {
                packet_ptr->ack_flag = 1;
                FDL_SendAckPacket (BSL_REP_ACK);
            }
            return 0;
        }
    }
#else

    if(s_flash_partition_info_ptr->NorBootloaderAdress == global_sys_stage.data.start_addr)
    {
        if(CheckSecureBootState())
        {
                    if (global_sys_stage.data .recv_size == ENCRYPT_OFF)
                    {
                    rda_encrypt_obj((char *) packet_ptr->packet_body.content, ENCRYPT_LEN);
                    }
        }
    }
#endif
#endif
        /*
        PRODUCTINFO 只能和 FIXNV 在一个sector, 并且该sector 不可以有其他如LCD DRIVE 之类的数据
        */
        if (g_is_dload_phasecheck == TRUE)
        {
            if (global_sys_stage.data .write_addr == s_flash_partition_info_ptr->ProdinfoAdress)
            {
                sector_size = FDL_FlashCheckSectorSize (s_flash_partition_info_ptr->NvAdress);
                //memcpy((uint8 *)SRAM_START_ADDR, (uint8 *)s_flash_partition_info_ptr->NvAdress , sector_size);
                FDL_FlashRead (s_flash_partition_info_ptr->NvAdress, (uint8 *) SRAM_START_ADDR, FIXNV_SECTOR_NUM*sector_size);
            }

            memcpy ( (uint8 *) (SRAM_START_ADDR + global_sys_stage.data .write_addr - s_flash_partition_info_ptr->NvAdress),
                     (char *) packet_ptr->packet_body.content,
                     data_len);

            global_sys_stage.data .write_addr = global_sys_stage.data .write_addr + data_len;
            global_sys_stage.data .recv_size  = global_sys_stage.data .recv_size + data_len;
            global_sys_stage.data .stage = SYS_STAGE_GATHER;

            if (!packet_ptr->ack_flag)
            {
                packet_ptr->ack_flag = 1;
                FDL_SendAckPacket (BSL_REP_ACK);
            }

            return 0;
        }

#else
        /*
        NOTE1: PRODUCT所在SECTOR数据组织可以是:
        (1)  FIXNV + PRODUCT INFO + 其它数据如:LCD DRIVE
        (2)  PRODUCT INFO + 其它数据如:LCD DRIVER
        (3)  FIXNV + PRODUCT INFO
        (4)  PRODUCT INFO
        */
        //***************hyy*****************
        //总能能公用SECTOR的形式 有两条
        // 1\  PRODUCTION INFO能和任何东西公用SECTOR
        // 2\  FIXNV不能和其他公用，除了PRODUCT INFO。且必须放在SECTOR头部

        uint32 product_info_addr = s_flash_partition_info_ptr->ProdinfoAdress;
        uint32  product_info_sec_size = 0;
        uint32 product_info_sec_addr = 0XFFFFFFFF;
        product_info_sec_size = FDL_FlashCheckSectorSize (product_info_addr);
        product_info_sec_addr = (product_info_addr/product_info_sec_size) * product_info_sec_size;

        if (product_info_sec_addr/product_info_sec_size == global_sys_stage.data .write_addr /product_info_sec_size)
        {
            if (g_is_product_sector ==0)
            {
                memcpy ( (uint8 *) SRAM_START_ADDR, (uint8 *) product_info_sec_addr , product_info_sec_size);
                slave_size = FDL_FlashCheckSectorSize (s_flash_partition_info_ptr->EfsAdress);

                if (slave_size > product_info_sec_size)
                {
                    for (j = product_info_sec_size; j < slave_size; j++)
                    {
                        * (uint8 *) (SRAM_START_ADDR+j) = 0xFF;
                    }
                }
            }

            g_is_product_sector = 1;

            if (global_sys_stage.data .write_addr < product_info_sec_addr)
            {
                SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
                handle_error();
            }

            memcpy ( (uint8 *) (SRAM_START_ADDR + global_sys_stage.data .write_addr - product_info_sec_addr), (char *) packet_ptr->packet_body.content, data_len);
            global_sys_stage.data .write_addr = global_sys_stage.data .write_addr + data_len;
            global_sys_stage.data .recv_size  = global_sys_stage.data .recv_size + data_len;
            global_sys_stage.data .stage = SYS_STAGE_GATHER;

            if ( ( (!packet_ptr->ack_flag) && (s_intel_m18_serial == 1)) || (g_is_dload_phasecheck == TRUE))
            {
                packet_ptr->ack_flag = 1;
                FDL_SendAckPacket (BSL_REP_ACK);
            }

            return 0;
        }

#endif


        if (s_is_dload_udisk_img)
        {
#ifdef UMEM_SUPPORT
#ifdef UMEM_NEW_MECHNISM
            UMEM_TYPE_E index;

            if (s_is_dload_udisk_img)
            {
                index = UMEM_TYPE_FS;
            }

            // FDL_SendAckPacket(0x99);
            FDL_Udisk_Img_Gen (index, global_sys_stage.data .write_addr, (char *) packet_ptr->packet_body.content, data_len);
            udisk_new_sceme = SCI_TRUE;
            // FDL_SendAckPacket(0x88);
#else
            g_umem_image_size += data_len;
            udisk_new_sceme = SCI_FALSE;
#endif
#endif
        }

        if (!udisk_new_sceme)
        {
            //write the flash
            if (FDL_FlashWriteArea (global_sys_stage.data .write_addr, (char *) packet_ptr->packet_body.content, data_len) != data_len)
            {
                SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);

                handle_error();
            }
        }

        if (!s_is_dload_udisk_img)
        {
            sector_size = FDL_FlashCheckSectorSize (global_sys_stage.data .write_addr);

            if ( (global_sys_stage.data .write_addr % sector_size) ==0)
            {
                if (global_sys_stage.data .write_addr > global_sys_stage.data .start_addr)
                {
                    if (!check_complete_sector (global_sys_stage.data .start_addr,global_sys_stage.data .write_addr-sector_size ,sector_size))
                    {
                        SEND_ERROR_RSP (BSL_WRITE_ERROR);
                        handle_error();
                    }
                }
            }

            record_samples (global_sys_stage.data.start_addr,global_sys_stage.data.write_addr,data_len, (char *) packet_ptr->packet_body.content);

            global_sys_stage.data .write_addr = global_sys_stage.data .write_addr + data_len;
            global_sys_stage.data .recv_size  = global_sys_stage.data .recv_size + data_len;
            global_sys_stage.data .stage      = SYS_STAGE_GATHER;

            // The ACK has been sent, needn't send it twice
            //FDL_SendAckPacket(BSL_REP_ACK);
        }
    }
    else   //It's fixnv data. We should backup it.
    {
        memcpy ( (uint8 *) g_sram_addr, (char *) packet_ptr->packet_body.content, data_len);
        g_sram_addr+=data_len;
    }

    if ( (!packet_ptr->ack_flag) && (1 == s_intel_m18_serial))
    {
        packet_ptr->ack_flag = 1;
        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 0;
}

int FDL_DataEnd (PACKET_T *packet_ptr,void *arg)
{
    uint8    *image_state_ptr=PNULL;
    uint8     set_size_buf[2];
    uint16   byte_per_sec;
    uint8    udisk_img_flag[6];
    uint16  *image_mapper_ptr=PNULL,*image_erasenum_ptr=PNULL,*image_excption_state_ptr=PNULL;
    uint32 i, fix_nv_checksum,udisk_size,udisk_img_base_addr, udisk_addr;
    uint32 umem_vir_sec_num,umem_data_sec_num,umem_phy_sec_num,umem_ctrl_stru_size,umem_sec_size,image_vir_sec_num;
    SEC_CTRL_T   *umem_info_ptr = PNULL;
    UMEM_CUSTDEFINE_T umem_cust_info ;

#ifdef SECURE_BOOT_SUPPORT
    uint8   *memAddr;
#endif

    static uint32 fix_nv_size = 0xFFFFFFFF;

    if (g_is_tfboot == 1)
    {
        g_is_tfboot = 0;
        goto finished;
    }

#ifdef UMEM_NEW_MECHNISM
#ifdef NOR_HIDDEN_UDISK_SUPPORT

    if (! (s_is_dload_fs_img||s_is_dload_hfs_img))
#else
        if (!s_is_dload_udisk_img)
#endif
#else
            if (!check_one_patition (global_sys_stage.data .start_addr,global_sys_stage.data .start_addr))
            {
                SEND_ERROR_RSP (BSL_WRITE_ERROR);
                handle_error();
            }

#endif

#ifndef UMEM_NEW_MECHNISM

    if (s_is_dload_udisk_img)
    {
#ifdef UMEM_CTRL_SEC_EN
        //step1 : init umem ctrl sec
        udisk_addr = FLASH_START_ADRESS + s_flash_partition_info_ptr->UmemAdress;
        umem_sec_size  = FDL_FlashCheckSectorSize (udisk_addr);
        umem_cust_info.umem_rsv_phy_sec_num = 1;
        umem_cust_info.umem_vir_sec_size = FLASH_GetUmemVirSize();

        FDL_FlashRead (udisk_addr + umem_sec_size + 11, set_size_buf, 2);
        byte_per_sec = (set_size_buf[1]<<8) | set_size_buf[0];

        if (byte_per_sec != umem_cust_info.umem_vir_sec_size)
        {
            send_ack (BSL_UDISK_VIR_SIZE_ERR);
        }

        umem_phy_sec_num  = s_flash_partition_info_ptr->UmemSize /umem_sec_size;
        umem_data_sec_num =  umem_phy_sec_num -  umem_cust_info.umem_rsv_phy_sec_num ;
        umem_vir_sec_num    = (umem_data_sec_num * umem_sec_size) / umem_cust_info.umem_vir_sec_size;
        umem_ctrl_stru_size   =  sizeof (SEC_CTRL_T)  + 3*umem_vir_sec_num + 2*umem_data_sec_num + 2;
        udisk_size  = g_umem_image_size + umem_sec_size ;//include ctrl sec and all data sec

        if (g_umem_image_size > (s_flash_partition_info_ptr->UmemSize - umem_sec_size))
        {
            SEND_ERROR_RSP (BSL_UDISK_IMAGE_SIZE_OVERFLOW);
            handle_error();
        }

        umem_info_ptr = (SEC_CTRL_T *)  SRAM_UMEM_START_ADDR;
        umem_info_ptr->sec_ctrl_start      = 0xfe;
        umem_info_ptr->sec_ctrl_stop       = 0xfe;
        umem_info_ptr ->sec_ctrl_index    = 0x0;

        umem_info_ptr->sec_ctrl_indicator =  s_flash_partition_info_ptr->UmemAdress/umem_sec_size;
        umem_info_ptr->sec_state            = (uint8 *) (s_flash_partition_info_ptr->UmemAdress  + sizeof (SEC_CTRL_T));
        umem_info_ptr->sec_mapper        = (uint16 *) (s_flash_partition_info_ptr->UmemAdress  + sizeof (SEC_CTRL_T) +umem_vir_sec_num);
        umem_info_ptr->sec_erasenum     = (uint16 *) (s_flash_partition_info_ptr->UmemAdress  + sizeof (SEC_CTRL_T) +umem_vir_sec_num*3);
        umem_info_ptr->sec_exception_state = (uint16 *) (s_flash_partition_info_ptr->UmemAdress +sizeof (SEC_CTRL_T) +3*umem_vir_sec_num + 2*umem_data_sec_num);

        FDL_memset ( (uint8 *) (SRAM_UMEM_START_ADDR + sizeof (SEC_CTRL_T)),   0x00, umem_vir_sec_num); //init sec state
        FDL_memset ( (uint8 *) (SRAM_UMEM_START_ADDR + sizeof (SEC_CTRL_T) + umem_vir_sec_num),  0xff, umem_vir_sec_num*2); //init sec mapper
        FDL_memset ( (uint8 *) (SRAM_UMEM_START_ADDR + sizeof (SEC_CTRL_T) + 3*umem_vir_sec_num),  0xff, umem_data_sec_num*2); //init sec erase num
        FDL_memset ( (uint8 *) (SRAM_UMEM_START_ADDR + sizeof (SEC_CTRL_T) + 3*umem_vir_sec_num + 2*umem_data_sec_num), 0xff, 2); //init sec exception state

        //step2:fill ctrl sec according to download umem image.
        image_vir_sec_num  = (udisk_size - umem_sec_size) / umem_cust_info.umem_vir_sec_size;

        if ( (udisk_size - umem_sec_size) % umem_cust_info.umem_vir_sec_size !=  0)
        {
            image_vir_sec_num++;
        }

        image_state_ptr       = (uint8 *) (SRAM_UMEM_START_ADDR  + sizeof (SEC_CTRL_T));
        image_mapper_ptr   = (uint16 *) (SRAM_UMEM_START_ADDR + sizeof (SEC_CTRL_T) + umem_vir_sec_num);
        image_erasenum_ptr= (uint16 *) (SRAM_UMEM_START_ADDR + sizeof (SEC_CTRL_T) + umem_vir_sec_num*3);
        image_excption_state_ptr = (uint16 *) (SRAM_UMEM_START_ADDR + sizeof (SEC_CTRL_T) + 3*umem_vir_sec_num + 2*umem_data_sec_num);

        for (i=0; i<image_vir_sec_num; i++)
        {
            image_state_ptr[i] = 0x02;
            image_mapper_ptr[i] = i;
        }

        //step3 : write ctrl sec info into flash.(ctrl sec info located at the beginning of udisk space)
        FDL_FlashErase (udisk_addr, umem_sec_size);

        if (!FDL_FlashWriteArea (udisk_addr, (char *) SRAM_UMEM_START_ADDR, umem_ctrl_stru_size))
        {
            SEND_ERROR_RSP (BSL_WRITE_ERROR);
        }

#else
        udisk_img_base_addr = FLASH_START_ADRESS + s_flash_partition_info_ptr->UmemAdress +  s_flash_partition_info_ptr->UmemSize - UMEM_IMG_INFO_SIZE;

        if (g_umem_image_size > (s_flash_partition_info_ptr->UmemSize - UMEM_IMG_INFO_SIZE))
        {
            SEND_ERROR_RSP (BSL_UDISK_IMAGE_SIZE_OVERFLOW);
            handle_error();
        }

        * (uint16 *) &udisk_img_flag[0] = UMEM_IMG_VALID_FLAG;
        * (uint16 *) &udisk_img_flag[2] = (g_umem_image_size>>16) & 0xffff ;
        * (uint16 *) &udisk_img_flag[4] = g_umem_image_size& 0xffff ;

        if (!FDL_FlashWriteArea (udisk_img_base_addr, (char *) &udisk_img_flag[0], UMEM_IMG_INFO_SIZE))
        {
            SEND_ERROR_RSP (BSL_WRITE_ERROR);
            handle_error();
        }

        global_sys_stage.data .stage      = SYS_STAGE_END;
        global_sys_stage.data_verify      = BSL_PHONE_SUCCEED;
#endif
    }

#endif

    if (CHECKSUM_OTHER_DATA != g_checksum)
    {
        fix_nv_size = g_sram_addr - SRAM_START_ADDR;

        fix_nv_checksum = Get_CheckSum ( (uint8 *) SRAM_START_ADDR, fix_nv_size);

        if (fix_nv_checksum != g_checksum)
        {
            SEND_ERROR_RSP (BSL_CHECKSUM_DIFF);
        }
#if 0
        if (s_share_fixnv_sector_flag == 1)
        {
            if (FDL_FlashCheckSectorSize (s_flash_partition_info_ptr->NvAdress) < fix_nv_size)
            {
                SEND_ERROR_RSP (BSL_WRITE_ERROR);
                handle_error();
            }

#ifndef FLASH_TYPE_SPI
            memcpy ( (void *) (SRAM_START_ADDR + fix_nv_size),
                     (void *) (s_flash_partition_info_ptr->NvAdress + fix_nv_size),
                     FDL_FlashCheckSectorSize (s_flash_partition_info_ptr->NvAdress) - fix_nv_size);
#endif
        }
#endif
        for (i=0; i<3; i++)
        {
            uint32 ret = 0;
            ret = update_fixnv ( (char *) SRAM_START_ADDR, fix_nv_size);

            if (ret)
            {
                break;
            }
            else
            {
                FDL_SendTrace ("ERROR! %s %d, ret=%d", __func__, __LINE__, ret);
            }
        }

        if (i==3)
        {
            //Write error happened
            SEND_ERROR_RSP (BSL_WRITE_ERROR);
        }

        g_write_imei_protect_flag |= FLAG_NV_DOWN;

    }
    else
    {
#ifdef FLASH_TYPE_SPI

        if (g_is_dload_phasecheck == TRUE)
        {

            for (i=0; i<3; i++)
            {
                uint32 ret = 0;
                ret = update_fixnv ( (char *) SRAM_START_ADDR, fix_nv_size);

                if (ret)
                {
                    break;
                }
                else
                {
                    FDL_SendTrace ("ERROR! %s %d, ret=%d", __func__, __LINE__, ret);
                }
            }

            if (i==3)
            {
                //Write error happened
                SEND_ERROR_RSP (BSL_WRITE_ERROR);
            }

            g_is_dload_phasecheck = FALSE;
        }

#else

        if (g_is_product_sector == 1)
        {
            for (i=0; i<3; i++)
            {
                if (update_productinfo ( (char *) SRAM_START_ADDR,  global_sys_stage.data .recv_size))
                {
                    break;
                }
            }

            if (i==3)
            {
                //Write error happened
                SEND_ERROR_RSP (BSL_WRITE_ERROR);
            }

            g_is_product_sector= 0;
        }

#endif

        //@hyy ps下结束了，判断FDL FLASH CFG与PS中的FLASH CFG是否一致
        if (global_sys_stage.data .start_addr == s_flash_partition_info_ptr->PsAdress)
        {
#if !defined(PLATFORM_UWS6121E)
#ifdef FLASH_TYPE_SPI
            if (0 != memcmp (pSpiFlashSpec[0]->spiflash_cfg, get_flash_partition_info_InPS(), sizeof (NOR_FLASH_CONFIG_T)))
#else
                if (0 != memcmp (s_normcp_entity_ptr->nor_flash_cfg_ptr, get_flash_partition_info_InPS(), sizeof (NOR_FLASH_CONFIG_T)))
#endif
                {
                    SEND_ERROR_RSP (BSL_FLASH_CFG_ERROR);

                    handle_error();
                }

            g_write_imei_protect_flag |= FLAG_PS_DOWN;
#endif

#ifdef SECURE_BOOT_SUPPORT
              //bootGetEfuseCtx();
             memAddr = (uint8 *)s_flash_partition_info_ptr->PsAdress;
        if (!bootSimageCheckSign((const simageHeader_t *)(memAddr)))
        {
            loop_var1 = 1;
            rprintf("PS secure boot check fail\n");
            FDL_SendAckPacket(BSL_REP_SECURITY_VERIFICATION_FAIL);
            while(loop_var1 == 1)
            {
                ;//Do Nothing
            }
        }
        rprintf("FDL_DataEnd: PS SECURE_BOOT_SUPPORT verify success\n");
#endif

        }

#ifdef SECURE_BOOT_SUPPORT
    if(s_flash_partition_info_ptr->NorBootloaderAdress == global_sys_stage.data.start_addr)
    {
        if (!bootSimageCheckSign((const simageHeader_t *)(global_sys_stage.data.start_addr)))
        {
            loop_var1 = 1;
            rprintf("NOR_BOOTLOADER secure boot check fail\n");
            FDL_SendAckPacket(BSL_REP_SECURITY_VERIFICATION_FAIL);
            while(loop_var1 == 1)
            {
                ;//Do Nothing
            }
        }
        rprintf("FDL_DataEnd: NOR_BOOTLOADER verify success\n");
    }
#endif

#ifndef STONE_IMAGE_SUPPORT

        if (global_sys_stage.data .start_addr == s_flash_partition_info_ptr->ResAdress)
        {
            g_write_imei_protect_flag |= FLAG_RES_DOWN;
        }

#endif

#ifndef MODEM_TYPE_ZBX
        if (global_sys_stage.data .start_addr == s_flash_partition_info_ptr->PsAdress)
        {
#ifdef FLASH_TYPE_SPI
#ifndef FOTA_SUPPORT_NONE
#else
            FDL_Mark_SPIFlash_Type();
#endif
#endif
        }
#endif
    }

finished:
    rprintf("FDL_DataEnd: BSL_PHONE_SUCCEED\n");
    global_sys_stage.data .stage      = SYS_STAGE_END;
    global_sys_stage.data_verify      = BSL_PHONE_SUCCEED;

    // send a ack immediatelly to allow PC to send next packet.
    if (!packet_ptr->ack_flag)
    {
        packet_ptr->ack_flag = 1;

        FDL_SendAckPacket (BSL_REP_ACK);
    }

    return 0;
}


extern void JumpToTarget (int Start_Addr);

int FDL_DataExec (PACKET_T *packet_ptr,void *arg)
{


    if (global_sys_stage.data_verify == BSL_REP_NOT_VERIFY)
    {
        SEND_ERROR_RSP (BSL_REP_NOT_VERIFY);
    }
    else
    {
        FDL_SendAckPacket (BSL_REP_ACK);

        /* jump to FDL */
        JumpToTarget (global_sys_stage.data .start_addr);


    }

    return 0;
}

//@ Liu kai 2003-11-05 CR5457
// Code provied by Leo Feng.
int FDL_McuResetNormal (PACKET_T *packet_ptr ,void *arg)
{
    FDL_SendAckPacket (BSL_REP_ACK);

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    CHIP_SOFT_Reset();
#elif !defined(PLATFORM_UWS6121E)
  {
    int i;

    FDL_SendAckPacket (BSL_REP_ACK);

    // Wait until the command is completed
    for (i = 0; i < 0x0A000; i++);

    reset_mcu_clock();

    {
        typedef void (*BOOT_RESET) (unsigned long bBoot);

        BOOT_RESET   boot_reset = (BOOT_RESET) (0x40000000);

        unsigned long *BootCodeAddr = (unsigned long *) 0x40000000;

        const long BootCode[] = {0xE59F1008, 0xE3A02000, 0xE5812000, 0xE1A0F000,
                                 0x20000118
                                };

        FDL_memcpy ( (void *) BootCodeAddr, BootCode, sizeof (BootCode));

        //Reset mcu with normal mode
        boot_reset (0);
    }
  }
#endif
    return 0;
}


#define FDL_FLASH_TEST_ENABLE 0
#if FDL_FLASH_TEST_ENABLE                           //for w217 flash test
#define TEST_ADDR_FLASH1 0X60000000
#define TEST_ADDR_FLASH2 0X70000000
void FDL_Flash_Test(unsigned int Addr)
{
    int i;
    int j=0;
    //Enable Write Flash
    rprintf("FDL_Flash_Test +++[0x%x]\n",Addr);

    rprintf("FDL_FlashWriteEnable 1\n");
    FDL_FlashWriteEnable (get_flash_base_addr (Addr), 1);

    //rprintf("FDL_FlashErase\n");
    FDL_FlashErase (Addr, 1024); //擦除完毕后数据默认为0XFF

    j=0;
    for(i=1023;i>=0;i--) //test data
    {
        erase_buf[j] = i&0x00ff;
        j++;
    }

#if 1
    rprintf("FDL_FlashWriteArea\n");
    if (FDL_FlashWriteArea (Addr, (char *) &erase_buf[0], M18_REGION_SIZE) != M18_REGION_SIZE)
    {
        rprintf("FDL_FlashWrite-ERR\n");
        //while(1);
    }
    else rprintf("FDL_FlashWrite-OK\n");
#endif

    for(i=0;i<1024;i++) //test data
    {
        rprintf("Read[0x%x]=0x%x\n",(Addr+i),*((unsigned char *)(Addr+i)));
    }

    rprintf("FDL_Flash_Test ---\n");

}
#endif

int  FDL_McuReadFlash (PACKET_T *packet_ptr ,void *arg)
{
    unsigned long  start_addr;
    unsigned long  read_length;
    uint32 offset_addr;
    unsigned long *ptr;

#if FDL_FLASH_TEST_ENABLE                           //for w217 flash test
    FDL_Flash_Test(TEST_ADDR_FLASH1);
    FDL_Flash_Test(TEST_ADDR_FLASH2);
#endif

    ptr = (unsigned long *) packet_ptr->packet_body.content;
    /* analyse the packet*/
    //start_addr   = *(unsigned long *)&pkt_buf[(int)PKT_HEADER_SIZE];
    //read_length    = *(unsigned long *)&pkt_buf[(int)PKT_HEADER_SIZE + sizeof(unsigned long)];
    start_addr  = *ptr++;
    start_addr = EndianConv_32 (start_addr);
    read_length = *ptr;
    read_length = EndianConv_32 (read_length);

    rprintf("%s: start_addr 0x%x, read_length %u\n", __func__, start_addr, read_length);

    ptr++;
    offset_addr = *ptr;
    offset_addr = EndianConv_32 (offset_addr);

    if (NV_LOGIC_ADRESS == start_addr)
    {
        //rprintf("NV_LOGIC_ADRESS start");
        start_addr = get_valid_nv_address();
        //rprintf("NV_LOGIC_ADRESS addr[0x%x]\n",start_addr);

        if (start_addr == 0xffffffff)
        {
            //SEND_ERROR_RSP (BSL_REP_VERIFY_ERROR);
            start_addr = s_flash_partition_info_ptr->NvAdress;

        }
        //rprintf("start_addr = 0x%x\n ",(unsigned long)(start_addr));
        start_addr += offset_addr;
    }
    else if (PHASECHECK_LOGIC_ADRESS ==  start_addr)
    {
        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            return 0;
        }

        start_addr = s_flash_partition_info_ptr->ProdinfoAdress;
        start_addr += offset_addr;
    }

#ifdef NOR_BOOTLOADER_SUPPORT
    else if (NOR_BOOTLOADER_LOGIC_ADRESS ==  start_addr)
    {
        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            return 0;
        }

        start_addr = s_flash_partition_info_ptr->NorBootloaderAdress;
        start_addr += offset_addr;
    }

#endif
    else if (PS_LOGIC_ADRESS ==  start_addr)
    {
        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            return 0;
        }

        start_addr = s_flash_partition_info_ptr->PsAdress;
        start_addr += offset_addr;
    }
    else if (RES_LOGIC_ADRESS == start_addr)
    {
        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            return 0;
        }

        start_addr = s_flash_partition_info_ptr->ResAdress;
        start_addr += offset_addr;
    }
    else if (RUNNING_NV_LOGIC_LOCAD_ADRESS ==  start_addr)
    {
        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            return 0;
        }

        start_addr = s_flash_partition_info_ptr->EfsAdress;
        start_addr += offset_addr;
    }
    else if (UMEM_LOGIC_LOAD_ADRESS == start_addr)
    {
        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            return 0;
        }

        start_addr = FLASH_START_ADRESS + s_flash_partition_info_ptr->UmemAdress;
        start_addr += offset_addr;
    }

    //rprintf("check_start_address\n ");
    if (!check_start_address (start_addr))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
        return 0;
    }

    //rprintf("check_access_size\n ");
    if (!check_access_size (start_addr, read_length)
            || (read_length > MAX_PKT_SIZE))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    packet_ptr->packet_body.type = BSL_REP_READ_FLASH;
    packet_ptr->packet_body.size = read_length;
    rprintf("FDL_memcpy sta_addr[0x%x] len[0x%x]\n",start_addr,read_length);
    FDL_memcpy (packet_ptr->packet_body.content, (char *) (start_addr), read_length);

    //rprintf("FDL_SendPacket\n ");
    FDL_SendPacket (packet_ptr);
    return 0;
}

int FDL_McuReadChipType (PACKET_T *packet_ptr,void *arg)
{
    uint32 id;

    id =  * (volatile uint32 *) (0xFFFFFFFC);

    packet_ptr->packet_body.type = BSL_REP_READ_CHIP_TYPE;
    packet_ptr->packet_body.size = 4;
    FDL_memcpy (packet_ptr->packet_body.content, &id, 4);

    FDL_SendPacket (packet_ptr);

    return 0;
}

static uint16 flash_get_checksum (uint32 addr, uint32 len)
{
    uint16  check_sum = 0xC50F + (uint16) addr;
    uint16 i = 0;
    uint16 *p_data = NULL;
    p_data = (uint16 *) (addr);
    while (len--)
    {
        check_sum += * (volatile uint16 *) (addr);
        addr      += sizeof (uint16);
    }
#if 0
    i++;
    if(( i % 8) == 0)
    {
        rprintf ("  %x %x %x %x %x %x %x %x \n",p_data[0], p_data[1],p_data[2],p_data[3],p_data[4], p_data[5], p_data[6],p_data[7] );
       p_data = (uint16 *) (addr);
    }
   }
     i = i % 8;
    if(( i % 7) == 0)
    {
        rprintf ("  %x %x %x %x %x %x %x \n",p_data[0], p_data[1],p_data[2],p_data[3],p_data[4], p_data[5], p_data[6]);
       goto last;
    }

    if(( i % 6) == 0)
    {
        rprintf ("  %x %x %x %x %x %x \n",p_data[0], p_data[1],p_data[2],p_data[3],p_data[4], p_data[5]);
       goto last;
    }

    if(( i % 5) == 0)
    {
        rprintf ("  %x %x %x %x %x \n",p_data[0], p_data[1],p_data[2],p_data[3],p_data[4]);
       goto last;
    }

    if(( i % 4) == 0)
    {
        rprintf ("  %x %x %x %x \n",p_data[0], p_data[1],p_data[2],p_data[3]);
       goto last;
    }

    if(( i % 3) == 0)
    {
        rprintf ("  %x %x %x \n",p_data[0], p_data[1],p_data[2]);
       goto last;
    }

    if(( i % 2) == 0)
    {
        rprintf ("  %x %x \n",p_data[0], p_data[1]);
       goto last;
    }

    if(( i % 1) == 0)
    {
        rprintf ("  %x \n",p_data[0]);
       goto last;
    }

 last:
#endif
    return check_sum;
}

// Search the given id in the given sector contain the address.
static uint32 search_nvitem_in_a_sector (uint32 addr, uint16 id)
{
    uint32            sector_size, end_addr;
    uint16            item_len, item_id, check_sum;

    sector_size = FDL_FlashCheckSectorSize (addr);
    addr &= ~ (sector_size - 1); // sector boundary
    end_addr = addr + sector_size - 6; // The max address contain nvitem.

    while (addr < end_addr)
    {
        item_id = * (volatile uint16 *) (addr);

        if (0xFFFF == item_id)
        {
            return FDL_INVALID_ADDR;  // reach tail, stop
        }
        else
        {
            // Check it is a valid item ?
            item_len  = * (volatile uint16 *) (addr + 6);
            check_sum = flash_get_checksum (addr + 4, (item_len + 4 + 1) >> 1);

            if (check_sum != * (volatile uint16 *) (addr + 2))
            {
                // Invalid item id
                return FDL_INVALID_ADDR;
            }
            else
            {
                // Valid
                if (item_id == id)
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
//void lookup_nvitem( PACKET_T * packet_ptr );
int FDL_LookupNvitem (PACKET_T *packet_ptr , void *arg)
{
    uint32  start_addr, end_addr, nv_addr;
    uint16  nv_id, nv_len;
    unsigned long *ptr;

    /* analyse the packet*/
    ptr = (unsigned long *) (packet_ptr->packet_body.content);
    start_addr = *ptr++;
    end_addr   = *ptr++;
    nv_id = * ( (unsigned short *) ptr);


    //Init Flash
    FDL_FlashWriteEnable (get_flash_base_addr (start_addr), 1);

    nv_addr = FDL_INVALID_ADDR;
    nv_len  = 0xFFFF;

    while (start_addr < end_addr)
    {
        // search in a sector
        nv_addr = search_nvitem_in_a_sector (start_addr, nv_id);

        if (FDL_INVALID_ADDR == nv_addr)
        {
            // Not found, try find next sector
            start_addr += FDL_FlashCheckSectorSize (start_addr);
        }
        else
        {
            // Found, return the address/item length
            nv_len  = * (volatile uint16 *) (nv_addr + 6);
            nv_len += 8 + (nv_len & 0x1);
            break;
        }
    }

    //memcpy( &rep_ack[DATA_ADDR ], &nv_addr, sizeof(nv_addr) );
    //memcpy( &rep_ack[DATA_ADDR + sizeof(nv_addr) ], &nv_len, sizeof(nv_len) );
    //((pkt_header_ptr)&rep_ack)->pkt_type = BSL_REP_LOOKUP_NVITEM;

    //packet_size                          = sizeof(nv_addr) + sizeof(nv_len);
    //((pkt_header_ptr)&rep_ack)->pkt_size = packet_size;
    //sio_write((char *)rep_ack,  PKT_HEADER_SIZE + packet_size + PKT_CRC_SIZE);

    packet_ptr->packet_body.type = BSL_REP_LOOKUP_NVITEM;
    packet_ptr->packet_body.size = sizeof (nv_addr) + sizeof (nv_len);
    memcpy (packet_ptr->packet_body.content, &nv_addr, sizeof (nv_addr));
    memcpy (& (packet_ptr->packet_body.content[sizeof (nv_addr) ]), &nv_len, sizeof (nv_len));
    FDL_SendPacket (packet_ptr);

    return 0;

}

int FDL_SetBaudrate (PACKET_T *packet_ptr , void *arg)
{

    unsigned long *ptr;
    unsigned long baudrate;

    ptr = (unsigned long *) packet_ptr->packet_body.content;

    baudrate = *ptr;
    baudrate = EndianConv_32 (baudrate);

    FDL_SendAckPacket (BSL_REP_ACK);

    gFdlUsedChannel->SetBaudrate (gFdlUsedChannel, baudrate);

    return 0;
}

int FDL_EraseFlash (PACKET_T *packet_ptr ,void *arg)
{
    unsigned long *ptr;
    unsigned long   start_addr, size;
#ifdef UMEM_NEW_MECHNISM
    static BOOLEAN is_efs_erased= SCI_FALSE;
#endif
    uint32 flashall = 0;

    ptr = (unsigned long *) packet_ptr->packet_body.content;

    start_addr = *ptr++;
    start_addr = EndianConv_32 (start_addr);
    size       = *ptr++;
    size = EndianConv_32 (size);

    rprintf("%s: start_addr 0x%x, size %d\n", __func__, start_addr, size);

    if (NV_LOGIC_ADRESS == start_addr)
    {
        start_addr = get_valid_nv_address();
        //start_addr = s_flash_partition_info_ptr->NvAdress;

        if (start_addr == 0xffffffff)
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
        }

        size = s_flash_partition_info_ptr->NvSize;
    }

    if (ERASEFLASH_LOGIC_ADRESS == start_addr)
    {
        if (!get_flash_partition_info (s_flash_partition_info_ptr))
        {
            SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
            return 0;
        }

#ifdef UMEM_NEW_MECHNISM

        if (!is_efs_erased)
        {
            is_efs_erased = SCI_TRUE;
        }
        else
        {
            FDL_SendAckPacket (BSL_REP_ACK);
            return 0;
        }

#endif
        start_addr = s_flash_partition_info_ptr->EfsAdress;
        size = s_flash_partition_info_ptr->EfsSize;
    }

    if (UMEM_LOGIC_ERASE_ADRESS == start_addr)
    {
#ifndef UMEM_NEW_MECHNISM
        if (0xFFFFFFFF != s_flash_partition_info_ptr->UmemAdress)
        {
            start_addr = FLASH_START_ADRESS + s_flash_partition_info_ptr->UmemAdress;
            size = s_flash_partition_info_ptr->UmemSize;
        }
        else
        {
            FDL_SendAckPacket (BSL_REP_ACK);
            return 0;
        }
#else
        if (!is_efs_erased)
        {
            is_efs_erased = SCI_TRUE;
            start_addr = s_flash_partition_info_ptr->EfsAdress;
            size = s_flash_partition_info_ptr->EfsSize;
        }
        else
        {
            FDL_SendAckPacket (BSL_REP_ACK);
            return 0;
        }
#endif
    }



    if (IMS_TOGGLE_LOGIC_ERASE_ADRESS == start_addr)
    {
        //rprintf("IMS_TOGGLE_LOGIC_ERASE_ADRESS == start_addr\n");
        if (0xFFFFFFFF != s_flash_partition_info_ptr->ims_toggle_addr)
        {
            start_addr = FLASH_START_ADRESS + s_flash_partition_info_ptr->ims_toggle_addr;
            size = s_flash_partition_info_ptr->ims_toggle_size;
            //rprintf("IMS_TOGGLE_LOGIC_ERASE_ADRESS: start 0x%x, size 0x%x\n", start_addr, size);
        }
        else
        {
            //rprintf("IMS_TOGGLE_LOGIC_ERASE_ADRESS == start_addr -ACK\n");
            FDL_SendAckPacket (BSL_REP_ACK);
            return 0;
        }
    }


    if (CONFIG_NV_LOGIC_ERASE_ADRESS == start_addr)
    {
        if (0xFFFFFFFF != s_flash_partition_info_ptr->config_nv_addr)
        {
            start_addr = FLASH_START_ADRESS + s_flash_partition_info_ptr->config_nv_addr;
            size = s_flash_partition_info_ptr->config_nv_size;
        }
        else
        {
            FDL_SendAckPacket (BSL_REP_ACK);
            return 0;
        }
    }



    if ((0 == start_addr) && (0xFFFFFFFF == size))
    {
        #ifdef NOR_BOOTLOADER_SUPPORT
        start_addr = s_flash_partition_info_ptr->NorBootloaderAdress;
        #else
        start_addr = s_flash_partition_info_ptr->PsAdress;
        #endif
        size = s_flash_partition_info_ptr->FlashSize;

        flashall = 1;
    }

    if (!check_start_address (start_addr))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
        return 0;
    }

    if (!check_access_size (start_addr, size))
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
        return 0;
    }

    //Enable Write Flash
    FDL_FlashWriteEnable (get_flash_base_addr (start_addr), 1);

    //FDL_FlashErase (start_addr, size);

    if ( (FLASH_START_ADRESS + s_flash_partition_info_ptr->UmemAdress) == start_addr)
    {
#ifndef UMEM_NEW_MECHNISM
        uint32 umem_sector_size;

        erase_buf[0] = UMEM_ERASE_FLAG;

        //write umem erased flag,will be used in powerup state.
        if (FDL_FlashWriteArea (start_addr, (char *) &erase_buf[0], M18_REGION_SIZE) != M18_REGION_SIZE)
        {
            SEND_ERROR_RSP (BSL_WRITE_ERROR);
            handle_error();
        }

        umem_sector_size = FDL_FlashCheckSectorSize (s_flash_partition_info_ptr->UmemAdress);
#else

        if (!is_efs_erased)
        {
            is_efs_erased = SCI_TRUE;
            FDL_FlashErase (start_addr, size);
        }

#endif
    }
    else
    {
#if defined(MULTI_FLASH_SUPPORT)
        if (flashall == 1)
        {
            flashall = 0;
            rprintf("FDL_FlashErase: start 0x%x, size 0x%x\n", start_addr, size);
            FDL_FlashErase (start_addr, size);
            rprintf("FDL_FlashErase: start 0x%x, size 0x%x\n", 0x70000000, 0x00400000);
            FDL_FlashErase (0x70000000, 0x00400000);
        }
        else
        {
            rprintf("really ERASE_ADDR 0x%x, really ERASE SIZE %d\n", start_addr, size);
            FDL_FlashErase (start_addr, size);
        }
    rprintf("FDL_FlashErase: OK !\n");

#else
        FDL_FlashErase (start_addr, size);
#endif
    }

    if (start_addr == s_flash_partition_info_ptr->EfsAdress)
    {
        g_write_imei_protect_flag |= FLAG_FLASH_DOWN;
    }

    if (g_write_imei_protect_flag == FLAG_ALL_OK)
    {
        if (s_intel_m18_serial == 0)
        {
            uint32 imei_flag_offset_addr;
            uint16 value;

            imei_flag_offset_addr = s_flash_partition_info_ptr->NvAdress +
                                    FDL_FlashCheckSectorSize (s_flash_partition_info_ptr->NvAdress) - FIX_NV_IMEI_FLAG_OFFSET;
            value = FIX_NV_IMEI_FLAG_MAGIC;

            FDL_FlashWriteArea (imei_flag_offset_addr, (char *) &value, sizeof (uint16));
        }
    }

    rprintf("FDL_FlashErase:addr[0x%x]size[0x%x]\n",start_addr,size);

    FDL_SendAckPacket (BSL_REP_ACK);

    return 0;
}


int FDL_PowerOff (PACKET_T *packet_ptr ,void *arg)
{
    if (!packet_ptr->ack_flag)
    {
        packet_ptr->ack_flag = 1;
    }

    FDL_SendAckPacket (BSL_REP_ACK);
    rprintf ("\nFDL_PowerOff ack!\n");
    do{
        /* waiting for usb plug out */
        gFdlUsedChannel->GetSingleChar (NULL); /* maybe pc will send cmd:SET_CTRL_LINE_STATE */
    } while(ANA_REG_GET(ANA_CHGR_STATUS) & CHGR_ON);

    CHIP_PHY_PB1KeyHwPD();
    while(1);
}

/*
int FDL_PowerOff (PACKET_T *packet_ptr ,void *arg)
{
    rprintf ("\n FDL_PowerOff enter!\n");
    packet_ptr->ack_flag = 0;
    if (!packet_ptr->ack_flag)
    {
        packet_ptr->ack_flag = 1;

        FDL_SendAckPacket (BSL_REP_ACK);
        rprintf ("\n FDL_PowerOff ack!\n");
    }


    while(1);
    do{
        ;  //rprintf ("ANA_CHGR_STATUS=[0x%x]!\n",(ANA_REG_GET(ANA_CHGR_STATUS)));
    } while(ANA_REG_GET(ANA_CHGR_STATUS) & CHGR_ON);

    //rprintf ("\n CHIP_PHY_PB1KeyHwPD ++!\n");
    CHIP_PHY_PB1KeyHwPD();
    while(1);
}
*/


//@Hyy->去FLASH CFG顺序的假设
//@含SORT功能，SORT when Insert复杂度低于SORT，本身就有INSERT功能即可用之
uint32 add_flash_partition_info (uint32 partition_addr)
{
    uint32 return_addr = partition_addr;
    uint32 i =0;
    uint32 j =0;

    if (partition_addr == 0xffffffff)
    {
        //invalid setting or default setting
        return 0xffffffff;
    }

    if (s_flash_partition_info_num == 0)
    {
        s_flash_partition_info_table[0] = partition_addr;
        s_flash_partition_info_num++;
        return return_addr;
    }

    if (s_flash_partition_info_num_in_cfg == s_flash_partition_info_num)
    {
        return return_addr;
    }

    //@快速通道
    if (partition_addr > s_flash_partition_info_table[s_flash_partition_info_num - 1])
    {
        s_flash_partition_info_table[s_flash_partition_info_num] = partition_addr;
        s_flash_partition_info_num++;
        return return_addr;
    }

    for (i=0; i< s_flash_partition_info_num; ++i)
    {
        if (partition_addr < s_flash_partition_info_table[i])
        {
            for (j = s_flash_partition_info_num; j > i; j--)
            {
                s_flash_partition_info_table[j] = s_flash_partition_info_table[j-1];
            }

            s_flash_partition_info_table[i] = partition_addr;
            s_flash_partition_info_num++;
            return return_addr;
        }
        else if (partition_addr == s_flash_partition_info_table[i])
        {
            SEND_ERROR_RSP (BSL_FLASH_CFG_ERROR);
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
uint32 get_flash_partition_size (uint32 partition_addr)
{
    uint32 i =0;

    if (partition_addr == 0xffffffff)
    {
        //invalid setting or default setting, return size 0 directly
        return 0;
    }

    if (s_flash_partition_info_num== 0)
    {
        SEND_ERROR_RSP (BSL_FLASH_CFG_ERROR);
        return 0;
    }

    for (i=0; i< s_flash_partition_info_num; i++)
    {
        if (partition_addr == s_flash_partition_info_table[i])
        {
            if (i== (s_flash_partition_info_num - 1))
            {
                //bottom
                return (SINGLE_CS_MAX_FLASH_SIZE - (partition_addr& (SINGLE_CS_MAX_FLASH_SIZE-1)));
            }
            else
            {
                if (s_flash_partition_info_table[i+1] <= s_flash_partition_info_table[i])
                {
                    SEND_ERROR_RSP (BSL_FLASH_CFG_ERROR);
                    return 0;
                }

                //@cross CS
                if ( (s_flash_partition_info_table[i+1] - s_flash_partition_info_table[i]) >= SINGLE_CS_MAX_FLASH_SIZE)
                {
                    return (SINGLE_CS_MAX_FLASH_SIZE - (partition_addr& (SINGLE_CS_MAX_FLASH_SIZE-1)));
                }
                else
                {
                    return (s_flash_partition_info_table[i+1] - s_flash_partition_info_table[i]);
                }
            }
        }
    }

    if (i== (s_flash_partition_info_num - 1))
    {
        SEND_ERROR_RSP (BSL_FLASH_CFG_ERROR);
        return 0;
    }

    return 0;
}
//@hyy<-end

#ifdef FLASH_TYPE_SPI

BOOLEAN  get_flash_partition_info (FLASH_PARTITION_PTR flash_partition_ptr)
{
    uint32 manufacturer_ID,device_ID,extend_ID;
    uint32 sect_size;
    uint32 cs = 0;

    if (flash_partition_ptr == 0)
    {
        return SCI_FALSE;
    }

    FDL_FlashWriteEnable (flash_infor[0].start_addr, 1);

    cs = SPIFLASH_CS0;
    pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get (nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);

    if (NULL ==  pSpiFlashSpec[cs])
    {
        return SCI_FALSE;
    }

    //@hyy ->modify for free cfg
#ifdef NOR_BOOTLOADER_SUPPORT
    flash_partition_ptr->NorBootloaderAdress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#ifdef SECURE_BOOT_SUPPORT
    flash_partition_ptr->PsAdress  = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->ps_addr);  //Secure Header of size SECURE_CODE_OFF is substracted in secure boot
#else
    flash_partition_ptr->PsAdress  = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->ps_addr);
#endif
#else
    flash_partition_ptr->PsAdress  = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#endif
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    flash_partition_ptr->FotaAdress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->fota_addr);
    flash_partition_ptr->FotaBootloaderAdress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->fota_bootloader_addr);
#endif
    flash_partition_ptr->ResAdress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->mmi_res);
    flash_partition_ptr->UmemAdress = add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->umem_addr); /*must not add FLASH_START_ADRESS !!!*/
    flash_partition_ptr->NvAdress  = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->fixnv_addr);
    flash_partition_ptr->ProdinfoAdress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->prodinfo_addr);
    flash_partition_ptr->EfsAdress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->efs_start_addr);
    flash_partition_ptr->Spload_Addr = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->spload_addr);
#ifdef SIM_LOCK_SUPPORT
    flash_partition_ptr->SimlockAddress= FLASH_START_ADRESS+add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->simlock_addr);
#endif
#ifdef DSP_DOWNLOAD_SUPPORT
    flash_partition_ptr->DspCodeAddress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->dsp_addr);
#endif
    flash_partition_ptr->OperatorDataAdress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->operator_data_addr);

#ifdef CHIP_VER_UWS6121E
    flash_partition_ptr->CpCodeAddress = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->cp_addr);
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    flash_partition_ptr->apn_addr = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->apn_addr);
#endif
    flash_partition_ptr->config_nv_addr = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->config_nv_addr);
    flash_partition_ptr->ims_toggle_addr = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->ims_toggel_cfg_addr);

#endif

    flash_partition_ptr->OperatorDataSize = pSpiFlashSpec[cs]->spiflash_cfg->operator_data_size;
    flash_partition_ptr->FlashSize = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

    s_flash_partition_info_num_in_cfg = s_flash_partition_info_num;
    //@修改为排序方式，后进行SIZE分配
    //@主推荐FLASH配置方式修改为PS-- RES --NORUMEM -FIXNV+PRODUCTINFO -EFS后的修改
#ifdef NOR_BOOTLOADER_SUPPORT
    flash_partition_ptr->NorBootloaderSize = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#ifdef SECURE_BOOT_SUPPORT
    flash_partition_ptr->PSSize    = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->ps_addr);  //Secure Header of size SECURE_CODE_OFF is substracted in secure boot
#else
    flash_partition_ptr->PSSize    = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->ps_addr);
#endif
#else
    flash_partition_ptr->PSSize    = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#endif
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    flash_partition_ptr->FotaSize = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->fota_addr);
    flash_partition_ptr->FotaBootloaderSize = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->fota_bootloader_addr);
#endif
//MMI RES patition in flash A, next patition is CP CODE in flash B
#if  defined( CHIP_VER_UWS6121E)
    flash_partition_ptr->ResSize   = pSpiFlashSpec[cs]->spiflash_cfg->mmi_res_size;
#else
       flash_partition_ptr->ResSize   = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->mmi_res);
#endif

    flash_partition_ptr->NvSize    = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->fixnv_addr);
    //@hyy<-End

    flash_partition_ptr->SploadSize   = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->spload_addr);
#ifdef SIM_LOCK_SUPPORT
    flash_partition_ptr->SimlockSize= get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->simlock_addr);
#endif

#if  defined(CHIP_VER_UWS6121E)
    flash_partition_ptr->CpCodeSize = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->cp_addr);
#ifdef FLASH_SIZE_128MBITX64MBIT_OLD
    flash_partition_ptr->apn_size = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->apn_addr);
#endif
    flash_partition_ptr->config_nv_size = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->config_nv_addr);
    flash_partition_ptr->ims_toggle_size=  (pSpiFlashSpec[cs]->spiflash_cfg->ims_toggel_cfg_size);

#endif
    //@根据其配的，不进行通用计算
    flash_partition_ptr->UmemSize  = pSpiFlashSpec[cs]->spiflash_cfg->umem_size;
#ifdef DSP_DOWNLOAD_SUPPORT
    flash_partition_ptr->DspCodeSize = pSpiFlashSpec[cs]->spiflash_cfg->dsp_size;
#endif

    // Calculate the efssize by sect_num multiply sect_size.
    flash_partition_ptr->EfsSize   = (pSpiFlashSpec[cs]->spiflash_cfg->sect_num) * pSpiFlashSpec[cs]->spiflash_cfg->sect_size;
    sect_size = pSpiFlashSpec[cs]->spiflash_cfg->sect_size;

#if 0
    if ( (flash_partition_ptr->ProdinfoAdress/sect_size) == (flash_partition_ptr->NvAdress/sect_size) + 1)
    {
        s_share_fixnv_sector_flag = 1;
    }
#endif
#ifdef FLASH_TYPE_SPI
#if 0
    else
    {
        FDL_SendTrace ("ERROR! ProdinfoAdress: 0x%x", flash_partition_ptr->ProdinfoAdress);

        // productinfo 和fixnv 必须共sector 否则不好给productinfo 和fixnv 同时做断电保护
        SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
    }
#endif
    g_prod_info_offset_addr = flash_partition_ptr->ProdinfoAdress - flash_partition_ptr->NvAdress;

    if (g_prod_info_offset_addr >= FIXNV_SECTOR_NUM*sect_size)
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
    }

    if ( (flash_partition_ptr->NvAdress + FIXNV_SECTOR_NUM*sect_size - flash_partition_ptr->ProdinfoAdress) <= PROD_INFO_MIN_SIZE_512)
    {
        FDL_SendTrace ("ERROR! ProdinfoAdress: 0x%x", flash_partition_ptr->ProdinfoAdress);

        // NPI tool 要求 productinfo size 必须大于4 KB, 否则CFT 工具和工厂工具会有问题。
        // 如果要修改这里的限制必须和NPI 人沟通并同步改NPI tool 的限制。
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
    }

#endif

    return TRUE;
}
#else
BOOLEAN  get_flash_partition_info (FLASH_PARTITION_PTR flash_partition_ptr)
{
    uint32  manufacturer_ID,device_ID,extend_ID;
    uint32 sect_size;

    if (flash_partition_ptr == 0)
    {
        return FALSE;
    }

    FDL_FlashWriteEnable (flash_infor[0].start_addr, 1);

    get_flash_id (&manufacturer_ID, &device_ID, &extend_ID);

    s_normcp_entity_ptr = NORMCP_GetEntity (manufacturer_ID,device_ID,extend_ID, PNULL);

    if (NULL ==  s_normcp_entity_ptr)
    {
        return NULL;
    }

    flash_partition_ptr->PsAdress  = FLASH_START_ADRESS + add_flash_partition_info (s_normcp_entity_ptr->nor_flash_cfg_ptr->start_addr);
    flash_partition_ptr->ResAdress = FLASH_START_ADRESS + add_flash_partition_info (s_normcp_entity_ptr->nor_flash_cfg_ptr->mmi_res);
    flash_partition_ptr->UmemAdress = add_flash_partition_info (s_normcp_entity_ptr->nor_flash_cfg_ptr->umem_addr); /*must not add FLASH_START_ADRESS !!!*/
    flash_partition_ptr->NvAdress  = FLASH_START_ADRESS + add_flash_partition_info (s_normcp_entity_ptr->nor_flash_cfg_ptr->fixnv_addr);
    flash_partition_ptr->ProdinfoAdress = FLASH_START_ADRESS + add_flash_partition_info (s_normcp_entity_ptr->nor_flash_cfg_ptr->prodinfo_addr);
    flash_partition_ptr->EfsAdress = FLASH_START_ADRESS + add_flash_partition_info (s_normcp_entity_ptr->nor_flash_cfg_ptr->efs_start_addr);
    flash_partition_ptr->Spload_Addr = FLASH_START_ADRESS + add_flash_partition_info (s_normcp_entity_ptr->nor_flash_cfg_ptr->spload_addr);
    flash_partition_ptr->FlashSize = s_normcp_entity_ptr->nor_flash_cfg_ptr->flash_size;

    s_flash_partition_info_num_in_cfg = s_flash_partition_info_num;
    //@modify sort method
    //@partiton sequencePS-- RES --NORUMEM -FIXNV+PRODUCTINFO -EFS

    flash_partition_ptr->PSSize    = get_flash_partition_size (s_normcp_entity_ptr->nor_flash_cfg_ptr->start_addr);

    flash_partition_ptr->ResSize   = get_flash_partition_size (s_normcp_entity_ptr->nor_flash_cfg_ptr->mmi_res);

    flash_partition_ptr->NvSize    = get_flash_partition_size (s_normcp_entity_ptr->nor_flash_cfg_ptr->fixnv_addr);
    //@hyy<-End

    flash_partition_ptr->SploadSize   = get_flash_partition_size (s_normcp_entity_ptr->nor_flash_cfg_ptr->spload_addr);

    flash_partition_ptr->UmemSize  = s_normcp_entity_ptr->nor_flash_cfg_ptr->umem_size;

    // Calculate the efssize by sect_num multiply sect_size.
    if (s_normcp_entity_ptr->normcp_spec_ptr->flash_drv_mode == FLASH_INTEL_M18_MODE)
    {
        flash_partition_ptr->EfsSize   = ( (s_normcp_entity_ptr->nor_flash_cfg_ptr->sect_num) >>1) * s_normcp_entity_ptr->nor_flash_cfg_ptr->sect_size;
    }
    else
    {
        flash_partition_ptr->EfsSize   = (s_normcp_entity_ptr->nor_flash_cfg_ptr->sect_num) * s_normcp_entity_ptr->nor_flash_cfg_ptr->sect_size;
    }

    sect_size = s_normcp_entity_ptr->nor_flash_cfg_ptr->sect_size;

    if ( (flash_partition_ptr->ProdinfoAdress/sect_size) == (flash_partition_ptr->NvAdress/sect_size))
    {
        s_share_fixnv_sector_flag = 1;
    }

    return TRUE;
}
#endif

#define MAX_FLASH_CODE_ADDR     (0x4000000)  //64MBit
#define MAX_ADDR_SEARCH_SIZE    0x4000
#define MIN_SECOR_SIZE          0x4000      //16KB
#define MIN_SECOR_MASK          (MIN_SECOR_SIZE - 1)
#define MAX_UINT32_VALUE        ((uint32)-1)

BOOLEAN __IsVaildSectorSize (uint32 sector_size)
{
    uint32 bit_cnt = MIN_SECOR_SIZE;

    if (!sector_size || (sector_size & MIN_SECOR_MASK))
    {
        return FALSE;
    }

    do
    {
        if (bit_cnt == sector_size)
        {
            return TRUE;
        }

        bit_cnt = bit_cnt << 0x01;
    }
    while ( (bit_cnt < MAX_UINT32_VALUE) && (bit_cnt != 0));

    return FALSE;
}

uint32  Equal_1K_LeftShift (uint32 count)
{
    uint32 i;

    for (i = 0; i<22; i++)
    {
        if ( (0x400<<i) == count)
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

PUBLIC NORFLASH_ENTITY_PTR NORMCP_GetEntityTabInPs (uint32 *entity_cnt, uint32 *entity_size, uint32 *cfg_offset)
{
#if defined(PLATFORM_UWS6121E)
    NORFLASH_ENTITY_PTR entity_tab_ptr = (NORFLASH_ENTITY_PTR) (FLASH_PS_START_ADRESS + FLASH_CFG_OFFSET);
    NOR_FLASH_CONFIG_PTR flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR) (FLASH_PS_START_ADRESS + FLASH_CFG_OFFSET);
#else
    NORFLASH_ENTITY_PTR entity_tab_ptr = (NORFLASH_ENTITY_PTR) (FLASH_START_ADRESS + FLASH_CFG_OFFSET);
    NOR_FLASH_CONFIG_PTR flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR) (FLASH_START_ADRESS + FLASH_CFG_OFFSET);
#endif
    uint32  i;
    uint32  total_sector_count;
    uint32 entity_element_size = 0;
    uint32 entity_flash_cfg_offset = 0;

#if 0
    i = 0;

    //@hyy First Check Entity Table In PS, Entity Element Size,because Entity Maybe Changed
    //@第一步 预查找，目的是找到FLASH_CFG的位置，其在ENTITY的结构位置不作假设，可以变
    //  找到到两个后认为OK
    {
        uint32 enter_addr = (uint32) entity_tab_ptr;
        uint32 flash_cfg_addr = enter_addr;
        VOLATILE NOR_FLASH_CONFIG_PTR _flash_cfg_ptr = 0;
        uint32 j =0;
        uint32 k=0;
        uint32 _flash_cfg_ptr_array[2] = {0};

        while ( (flash_cfg_addr -enter_addr) < MAX_SECARCH_RANGE)
        {
            flash_cfg_addr = enter_addr + (j<<2);
#if defined(PLATFORM_UWS6121E)
            _flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR) flash_cfg_addr;
#else
            _flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR) ( (* (uint32 *) flash_cfg_addr) +FLASH_START_ADRESS);
#endif

#if defined(PLATFORM_UWS6121E)
            if ( (_flash_cfg_ptr->start_addr == 0x60000000)
#else
            if ( (_flash_cfg_ptr->start_addr == 0)
#endif
                && Equal_1K_LeftShift (_flash_cfg_ptr->sect_size))
            {

                if ( (_flash_cfg_ptr->flash_size & (_flash_cfg_ptr->sect_size -1)) == 0) /*equal:flash_cfg_ptr->flash_size%flash_cfg_ptr->sect_size*/
                {
                    total_sector_count = _flash_cfg_ptr->flash_size/_flash_cfg_ptr->sect_size;

                    if ( (_flash_cfg_ptr->file_sect_num<total_sector_count) &&
                            (_flash_cfg_ptr->bank_num<=total_sector_count) &&
                            (_flash_cfg_ptr->sect_num<total_sector_count))
                    {

                        if ( ( (_flash_cfg_ptr->efs_start_addr& (_flash_cfg_ptr->sect_size-1)) ==0) &&
                                ( (_flash_cfg_ptr->fixnv_addr& (_flash_cfg_ptr->sect_size-1)) ==0))
                        {

                            //send_ack(0x97);
                            _flash_cfg_ptr_array[k] = flash_cfg_addr;
                            ++k;

                            if (k==1)
                            {
                                entity_flash_cfg_offset = (uint32) flash_cfg_addr - enter_addr;
                            }

                            if (k==2)
                            {
                                entity_element_size = _flash_cfg_ptr_array[1]- _flash_cfg_ptr_array[0];
                                break;
                            }
                        }
                    }

                }
            }

            j++;

            if ( (k==1) && (j>100))
            {
                break;
            }
        }

        if (k == 0)
        {
            *entity_cnt = 0;
            return NULL;
        }

        if (k == 1)
        {
            *entity_cnt = 1;
            *entity_size = 12;
            *cfg_offset = entity_flash_cfg_offset;
            return entity_tab_ptr;
        }
    }

    //@error
    if (entity_element_size < 12)
    {
        *entity_cnt = 0;
        return NULL;
    }

    if (entity_flash_cfg_offset>= entity_element_size)
    {
        *entity_cnt = 0;
        return NULL;
    }


    //@第二步: 找EntityTab里面Entity的数量
    {
        uint32 enter_addr = (uint32) entity_tab_ptr;
        uint32 flash_cfg_addr = enter_addr;
        i=0;

        while (1)
        {
            flash_cfg_addr = enter_addr + entity_element_size * i + entity_flash_cfg_offset;
            flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR) ( (* (uint32 *) flash_cfg_addr) +FLASH_START_ADRESS);

#if defined(PLATFORM_UWS6121E)
            if ( (flash_cfg_ptr->start_addr == 0x60000000)
#else
            if ( (flash_cfg_ptr->start_addr == 0)
#endif
                && Equal_1K_LeftShift (flash_cfg_ptr->sect_size))
            {
                if ( (flash_cfg_ptr->flash_size & (flash_cfg_ptr->sect_size -1)) == 0) /*equal:flash_cfg_ptr->flash_size%flash_cfg_ptr->sect_size*/
                {
                    total_sector_count = flash_cfg_ptr->flash_size/flash_cfg_ptr->sect_size;

                    if ( (flash_cfg_ptr->file_sect_num<total_sector_count) &&
                            (flash_cfg_ptr->bank_num<=total_sector_count) &&
                            (flash_cfg_ptr->sect_num<total_sector_count))
                    {

                        if ( ( (flash_cfg_ptr->efs_start_addr& (flash_cfg_ptr->sect_size-1)) ==0) &&
                                ( (flash_cfg_ptr->fixnv_addr& (flash_cfg_ptr->sect_size-1)) ==0))

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
    SCI_BootUp_UartTrace ("[Yiyue]: entity_tab_ptr=0x%x,%d,%d,%d", (uint32) entity_tab_ptr,i,entity_element_size,entity_flash_cfg_offset);
#endif
#endif

    NORFLASH_ENTITY_PTR entity_tab_tmp_ptr;
        i=0;
    entity_element_size = sizeof(struct  SPI_flash_spec_s);
        entity_flash_cfg_offset = (uint32)(&(((NORFLASH_ENTITY_PTR)0)->spiflash_cfg));
    entity_tab_tmp_ptr = entity_tab_ptr;
        while ((entity_tab_tmp_ptr->spiflash_cfg) || (entity_tab_tmp_ptr->pExtcfg))
        {

                 //restrick the check number
             if(i > 100)
             {
                 rprintf ("\nexceed the max check number!\n");
                break;
             }

                flash_cfg_ptr =  entity_tab_tmp_ptr->spiflash_cfg;
                entity_tab_tmp_ptr++;
                i++;


            if ( (flash_cfg_ptr->start_addr == 0x60000000)
                && Equal_1K_LeftShift (flash_cfg_ptr->sect_size))
            {
                if ( (flash_cfg_ptr->flash_size & (flash_cfg_ptr->sect_size -1)) == 0) /*equal:flash_cfg_ptr->flash_size%flash_cfg_ptr->sect_size*/
                {
                    total_sector_count = flash_cfg_ptr->flash_size/flash_cfg_ptr->sect_size;

                    if ( (flash_cfg_ptr->file_sect_num<total_sector_count) &&
                            (flash_cfg_ptr->bank_num<=total_sector_count) &&
                            (flash_cfg_ptr->sect_num<total_sector_count))
                    {

                        if ( ( (flash_cfg_ptr->efs_start_addr& (flash_cfg_ptr->sect_size-1)) ==0) &&
                                ( (flash_cfg_ptr->fixnv_addr& (flash_cfg_ptr->sect_size-1)) ==0))

                        {
                            continue;
                        }
                    }
                }
            }
        }


    *entity_cnt = i;
    *entity_size = entity_element_size;
    *cfg_offset = entity_flash_cfg_offset;
      rprintf ("\nentity_tab_ptr=0x%x entity_cnt=%d entity_size=%d cfg_offset=%d, %s %d\n", (uint32)entity_tab_ptr, i,entity_element_size,entity_flash_cfg_offset,__func__, __LINE__);

    return entity_tab_ptr;
}

#ifdef FLASH_TYPE_SPI
PUBLIC SPIFLASH_SPEC_PTR NORMCP_GetEntityInPs (
    uint32 manu_id,
    uint32 devc_id,
    uint32 extend_id,
    uint32 *index,
    uint32 *cfg_offset)
{
    uint32 table_index               = 0;
    uint32 table_num                 = 0;
    uint32 entity_size = 0;
    SPIFLASH_SPEC_PTR entity_ptr     = PNULL;
    SPIFLASH_SPEC_PTR entity_tab_ptr = NORMCP_GetEntityTabInPs (&table_num,&entity_size,cfg_offset);
    SPIFLASH_SPEC_PTR   flash_spec_ptr;

    for (table_index=0; table_index < table_num; ++table_index)
    {
        flash_spec_ptr = (SPIFLASH_SPEC_PTR) ( ( (uint32) ( (SPIFLASH_SPEC_PTR) ( ( (uint32) entity_tab_ptr) + (table_index*entity_size)))));

        if ( (manu_id   == flash_spec_ptr->nMID)
                && (devc_id   == flash_spec_ptr->nDID1)
                && (extend_id == flash_spec_ptr->nDID2))
        {
            //Find Nor Type
            entity_ptr = ( (SPIFLASH_SPEC_PTR) ( ( (uint32) entity_tab_ptr) + (table_index*entity_size)));
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
#else
PUBLIC NORMCP_ENTITY_PTR NORMCP_GetEntityInPs (
    uint32 manu_id,
    uint32 devc_id,
    uint32 extend_id,
    uint32 *index,
    uint32 *cfg_offset)
{
    uint32 table_index               = 0;
    uint32 table_num                 = 0;
    uint32 entity_size = 0;
    NORMCP_ENTITY_PTR entity_ptr     = PNULL;
    NORMCP_ENTITY_PTR entity_tab_ptr = NORMCP_GetEntityTabInPs (&table_num,&entity_size,cfg_offset);
    NORMCP_SPEC_PTR flash_spec_ptr;

    for (table_index=0; table_index < table_num; ++table_index)
    {
        flash_spec_ptr = (NORMCP_SPEC_PTR) ( (uint32) entity_tab_ptr[table_index].normcp_spec_ptr+FLASH_START_ADRESS);

        if ( (manu_id   == flash_spec_ptr->manu_id)
                && (devc_id   == flash_spec_ptr->devc_id)
                && (extend_id == flash_spec_ptr->extend_id))
        {
            //Find Nor Type
            entity_ptr = (NORMCP_ENTITY_PTR) &entity_tab_ptr[table_index];
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
#endif

NOR_FLASH_CONFIG_PTR get_flash_partition_info_InPS (void)
{
#ifdef FLASH_TYPE_SPI
    uint32 index;
    SPIFLASH_SPEC_PTR entity_ptr = PNULL;
    NOR_FLASH_CONFIG_PTR flash_cfg = PNULL;
    uint32 cfg_offset =0;
    uint32 cs = 0;

    FDL_FlashWriteEnable (flash_infor[0].start_addr, 1);
    cs= SPIFLASH_CS0;
    entity_ptr = NORMCP_GetEntityInPs (nID_MID[cs],nID_DID1[cs],nID_DID2[cs],&index,&cfg_offset);

    if (entity_ptr==NULL)
    {
        return NULL;
    }

    flash_cfg  = (NOR_FLASH_CONFIG_PTR) ( (* (uint32 *) ( ( (uint32) entity_ptr) +cfg_offset)) +FLASH_START_ADRESS);

    return flash_cfg;
#else
    uint32 manufacturer_ID,device_ID,extend_ID, index;
    //uint32 temp0, temp1;
    NORMCP_ENTITY_PTR entity_ptr = PNULL;
    NOR_FLASH_CONFIG_PTR flash_cfg = PNULL;
    uint32 cfg_offset =0;

    FDL_FlashWriteEnable (flash_infor[0].start_addr, 1);

    get_flash_id (&manufacturer_ID, &device_ID, &extend_ID);

    entity_ptr = NORMCP_GetEntityInPs (manufacturer_ID,device_ID,extend_ID,&index,&cfg_offset);

    if (entity_ptr==NULL)
    {
        return NULL;
    }

    flash_cfg  = (NOR_FLASH_CONFIG_PTR) (FLASH_START_ADRESS+ (uint32) entity_ptr->nor_flash_cfg_ptr);
    return flash_cfg;
#endif
}

uint32 calculate_fixNV_length (uint32  start_addr, uint32 end_addr)
{
    uint32  cur_addr;
    uint16  id, len;
    uint32  fix_dedicate_len;
    BOOLEAN is_sram_fixnv = SCI_FALSE;

    if ( (start_addr >= SRAM_START_ADDR) && (start_addr <= (SRAM_START_ADDR + SINGLE_CS_MAX_FLASH_SIZE)))
    {
        is_sram_fixnv = SCI_TRUE;
    }

    if (is_sram_fixnv)
    {
        fix_dedicate_len = * (uint32 *) (end_addr - FIX_NV_LEN_OFFSET);
    }
    else
    {
        fix_dedicate_len = FDL_FlashReadDWord (end_addr - FIX_NV_LEN_OFFSET);
    }

    cur_addr = start_addr + sizeof (uint32); // skip update flag.

    while (cur_addr < end_addr)
    {
        if (is_sram_fixnv)
        {
            id   = * (uint16 *) (cur_addr);
            len = * (uint16 *) (cur_addr+2);
        }
        else
        {
            id   = FDL_FlashReadWord (cur_addr);
            len = FDL_FlashReadWord (cur_addr+2);
        }

        //FDL_SendTrace("%s %d id=0x%x, len=0x%x 0x%x %d", __func__, __LINE__, id, len, cur_addr, is_sram_fixnv);

        cur_addr += 4;

        if (EFS_INVALID_ITEM == id)  // Reach tail
        {
            cur_addr -= 4;
            break; // Stop loop
        }
        else if (id > ITEM_MASK_ID)
        {
            FDL_SendTrace ("ERROR! %s %d id=0x%x, start_addr=0x%x", __func__, __LINE__, id, start_addr);
            return ERR_NV_LEN;
        }

        if ( (len == 0x0) || (id == 0))
        {
            FDL_SendTrace ("ERROR! %s %d id=0x%x, len=0x%x, start_addr=0x%x", __func__, __LINE__, id, len, start_addr);
            return ERR_NV_LEN;
        }

        cur_addr += len + (len & 0x1); // WORD boundary
        cur_addr +=3;
        cur_addr &=0xfffffffc;
    }

    if (fix_dedicate_len != (uint32) (cur_addr - start_addr))
    {
        FDL_SendTrace ("ERROR! %s %d fix_dedicate_len=0x%x, cur_addr=0x%x, start_addr=0x%x",
                       __func__, __LINE__, fix_dedicate_len, cur_addr, start_addr);
        return ERR_NV_LEN;
    }

    //FDL_SendTrace("%s start_addr=0x%x fix_dedicate_len=0x%x", __func__, start_addr, fix_dedicate_len);
    return fix_dedicate_len;
}

#ifndef FLASH_TYPE_SPI
BOOLEAN  lookup_valid_nv_address (uint32 *valid_nv_addr_out)
{
    uint32 end_addr;
    uint32 sect_size;
    uint16 sector_figure_crc;
    uint16 sector_dedicated_crc;
    uint32 sector_fixnv_size;
    BOOLEAN is_find_valid_nv = FALSE;
    NV_STATUS_TYPE_E sec_status;
    end_addr   =  FLASH_START_ADRESS + s_flash_partition_info_ptr->FlashSize;

    if (s_flash_partition_info_ptr->FlashSize > SINGLE_CS_MAX_FLASH_SIZE)
    {
        end_addr   =  FLASH_START_ADRESS + SINGLE_CS_MAX_FLASH_SIZE;

        while (end_addr > FLASH_START_ADRESS)
        {
            sect_size   = FDL_FlashCheckSectorSize (end_addr -1); //必须要减一
            end_addr -= sect_size;
            sec_status = FDL_FlashReadWord (end_addr + sect_size-SECTOR_STATUS_OFFSET);

            if ( (sec_status == NV_MINVALID_SOK) || (sec_status == NV_MOK_SINVALID) || \
                    ( (sec_status == NV_SOK_PRD_INFO) && (s_share_fixnv_sector_flag ==1)))
            {
                sector_dedicated_crc = FDL_FlashReadWord (end_addr);
                sector_fixnv_size = FDL_FlashReadWord (end_addr + sect_size - FIX_NV_LEN_OFFSET);

                if ( (sector_fixnv_size <= (sect_size - FIX_NV_LEN_OFFSET)) && (sector_fixnv_size >= sizeof (uint16)))
                {
                    sector_figure_crc = crc16 (0, (uint8 *) (end_addr + sizeof (uint16)), sector_fixnv_size - sizeof (uint16));

                    if ( (sector_dedicated_crc == sector_figure_crc) ||
                            (sector_dedicated_crc == EndianConv_16 (sector_figure_crc)))
                    {
                        *valid_nv_addr_out = end_addr;
                        is_find_valid_nv = TRUE;
                        break;
                    }
                }
            }
        }

        end_addr  =  FLASH_START_ADRESS_CS3 + SINGLE_CS_MAX_FLASH_SIZE;

        while (end_addr > FLASH_START_ADRESS_CS3)
        {
            sect_size   = FDL_FlashCheckSectorSize (end_addr -1); //必须要减一
            end_addr -= sect_size;
            sec_status = FDL_FlashReadWord (end_addr + sect_size-SECTOR_STATUS_OFFSET);

            if ( (sec_status == NV_MINVALID_SOK) || (sec_status == NV_MOK_SINVALID) || \
                    ( (sec_status == NV_SOK_PRD_INFO) && (s_share_fixnv_sector_flag ==1)))
            {
                sector_dedicated_crc = FDL_FlashReadWord (end_addr);
                sector_fixnv_size = FDL_FlashReadWord (end_addr + sect_size - FIX_NV_LEN_OFFSET);

                if ( (sector_fixnv_size <= (sect_size - FIX_NV_LEN_OFFSET)) && (sector_fixnv_size >= sizeof (uint16)))
                {
                    sector_figure_crc = crc16 (0, (uint8 *) (end_addr + sizeof (uint16)), sector_fixnv_size - sizeof (uint16));

                    if ( (sector_dedicated_crc == sector_figure_crc) ||
                            (sector_dedicated_crc == EndianConv_16 (sector_figure_crc)))
                    {
                        *valid_nv_addr_out = end_addr;
                        is_find_valid_nv = TRUE;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        while (end_addr > FLASH_START_ADRESS)
        {
            sect_size   = FDL_FlashCheckSectorSize (end_addr -1); //必须要减一
            end_addr -= sect_size;
            sec_status = FDL_FlashReadWord (end_addr + sect_size-SECTOR_STATUS_OFFSET);

            if ( (sec_status == NV_MINVALID_SOK) || (sec_status == NV_MOK_SINVALID) || \
                    ( (sec_status == NV_SOK_PRD_INFO) && (s_share_fixnv_sector_flag ==1)))
            {
                sector_dedicated_crc = FDL_FlashReadWord (end_addr);
                sector_fixnv_size = FDL_FlashReadWord (end_addr + sect_size - FIX_NV_LEN_OFFSET);

                if ( (sector_fixnv_size <= (sect_size - FIX_NV_LEN_OFFSET)) && (sector_fixnv_size >= sizeof (uint16)))
                {
                    sector_figure_crc = crc16 (0, (uint8 *) (end_addr + sizeof (uint16)), sector_fixnv_size - sizeof (uint16));

                    if ( (sector_dedicated_crc == sector_figure_crc) ||
                            (sector_dedicated_crc == EndianConv_16 (sector_figure_crc)))
                    {
                        *valid_nv_addr_out = end_addr;
                        is_find_valid_nv = TRUE;
                        break;
                    }
                }
            }
        }
    }

    return is_find_valid_nv;
}
#endif

uint32 Get_productinfo_offset (void)
{
    return g_prod_info_offset_addr;
}

uint32 Get_fdl_file_end_addr (void)
{
    return g_fdl_file_end_addr;
}

BOOLEAN Get_phasecheck_flag (void)
{
    return g_is_dload_phasecheck;
}

/*add dummy func here*/
void SCI_Assert (void *exp,  void *file, uint32  line) {}

