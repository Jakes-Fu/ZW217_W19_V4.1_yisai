/******************************************************************************
 ** File Name:      FDL_main.c                                                 *
 ** Author:                                                         *
 ** DATE:           12/15/2015                                               *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "fdl_stdio.h"
#include "fdl_command.h"
#include "fdl_conf.h"
#include "fdl_main.h"
#include "cmd_def.h"
#include "packet.h"
#include "dl_engine.h"
#include "sio_drv.h"
#include "usb_boot.h"
#include "virtual_com.h"
#include "fdl_channel.h"
#include "mem_cfg.h"

#ifdef PLATFORM_UWS6121E
#include "uws6121e_reg_analog.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
/*lint -e551 for symbol:"stack_space" */
#pragma arm section zidata = "ZIDATA_AREA_0"
LOCAL unsigned char stack_space[STACK_SIZE];
#pragma arm section zidata

uint32 s_intel_m18_serial = 0;
BOOLEAN  g_is_dload_phasecheck = FALSE;

const unsigned long stack_bottom = ( (unsigned long) &stack_space + STACK_SIZE);

static const char version_string[] = {"Spreadtrum Boot Block version 1.2"};

extern BOOLEAN g_is_keep_charge_enable;
/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
#if defined(MEM_STRESS_TEST_PSRAM) 
extern void  psram_test(void);
#endif

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
static void error (void)
{
    for (;;) /*Do nothing*/;
}

//#ifdef SECURE_BOOT_SUPPORT
#if 0
uint32 download_secure_check(uint8* secure_code_ptr, uint8* fdl_vlr_ptr)
{
#define CONFIG_SRAM_START               0x00800000
#define CONFIG_TRAP_SIZE                0xc0
#define CONFIG_LOAD_ADDRESS             (CONFIG_SRAM_START + CONFIG_TRAP_SIZE)
#define FDL_SIGNED_SIZE     (0xbce0)
#define NOR_BOOTLOADER_PUBKEY_ADDR (CONFIG_LOAD_ADDRESS + FDL_SIGNED_SIZE - KEY_SIZE)
    uint8  *puuk_ptr               = (uint8*)(NOR_BOOTLOADER_PUBKEY_ADDR);  //PUK is stored in bootloader
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

typedef enum uart_instance
{
    uart1_instance = 1,
    uart2_instance,
    uart3_instance,
    uart4_instance,
}uart_instance_t;


int main (void)
{
    PACKET_T *packet;
    char ch;
    char ch1, ch2;

    //ESAL_TS_RTE_CP_WRITE(p15, 0, 0x50800000, c12, c0, 0);
    //ESAL_TS_RTE_CP_WRITE(p15, 0, 0x50800000, c12, c0, 1);

    /*serial_init:init uart4 for log output(115200,baudrete has been configured in RomCode)*/
    //serial_init();
    serial_init(uart2_instance);

#ifdef MEM_STRESS_TEST_PSRAM
    psram_test();
    return;
#endif
     ADI_Phy_init();
    ANA_REG_SET(ANA_PWR_WR_PROT_VALUE, 0x6e7f);
    do
    {
        while (!(ANA_REG_GET(ANA_PWR_WR_PROT_VALUE) == 0x8000)){
        }
    } while (0);
     ANA_REG_OR(ANA_POWER_PD_SW0, LDO_VIO33_PD);
#if defined(MODEM_PLATFORM)
    ANA_REG_OR (ANA_POR_7S_CTRL,BIT_1); //disable 7s reset
#endif

#if defined(PLATFORM_UWS6121E)
    usb_boot (1);
#endif
    s_intel_m18_serial = 0;
    FDL_PacketInit();
    FDL_DlInit();

    FDL_DlReg (BSL_CMD_CONNECT,    NOR_FDL1_SysConnect,  0);
    FDL_DlReg (BSL_CMD_START_DATA, NOR_FDL1_DataStart,   0);
    FDL_DlReg (BSL_CMD_MIDST_DATA, NOR_FDL1_DataMidst,   0);
    FDL_DlReg (BSL_CMD_EXEC_DATA,  NOR_FDL1_DataExec,    0);
    FDL_DlReg (BSL_CMD_END_DATA,   NOR_FDL1_DataEnd,     0);
    FDL_DlReg (BSL_SET_BAUDRATE,   NOR_FDL1_SetBaudrate, 0);
    FDL_DlReg (BSL_CMD_KEEP_CHARGE,NOR_FDL1_KeepCharge,  0);

#if defined(PLATFORM_UWS6121E)
    rprintf("wait connect\n");
#endif


    for ( ; ; )
    {

#if defined(PLATFORM_UWS6121E)
        ch = gFdlUsedChannel->GetSingleChar (NULL);
#else
        ch = gFdlUsedChannel->GetChar(gFdlUsedChannel);
#endif

        if (0x7e == ch)
        {
#if defined(PLATFORM_UWS6121E)
            rprintf("Connected\n");
#endif
            break;
        }

    }

#if defined(PLATFORM_UWS6121E)
    ch1 = gFdlUsedChannel->GetSingleChar (NULL);
    ch2 = gFdlUsedChannel->GetSingleChar (NULL);
    if ((ch1 == 0x00) && (ch2 == 0x13))
    {
        rprintf("NOR_FDL1_KeepCharge\n");
        g_is_keep_charge_enable = TRUE;
        CHIP_PHY_AndANAReg (ANA_CHGR_CTL0, ~CHGR_PD); //keep charge
    }
#endif

    //when got the signal of 0x7e ,send response to pc
    packet = FDL_MallocPacket();
    packet->packet_body.type = BSL_REP_VER;
    packet->packet_body.size = sizeof (version_string);
    FDL_memcpy (packet->packet_body.content, version_string, sizeof (version_string));
    FDL_SendPacket (packet);
    FDL_FreePacket (packet);

    FDL_DlEntry (DL_STAGE_NONE);

    error();
    return 1;
}
