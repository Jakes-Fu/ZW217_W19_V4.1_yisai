/******************************************************************************
 ** File Name:      pin_reg_v0.h                                      *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           07/18/2008                                                *
 ** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 **                 Pinmap Register address map for the sc6600l chip                 *
 **                 Reference to the SC6600L control register document        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/17/2008     Liangwen.Zhen        Create.                                   *
 ******************************************************************************/
#ifndef _PIN_REG_V0_H_
#define _PIN_REG_V0_H_

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define PINMAP_REG_ADDR(_x_)        (PINMAP_REG_BASE + _x_)

#define PIN_CTL_REG                 PINMAP_REG_ADDR(0x00)
#define PIN_CCIRCK_REG              PINMAP_REG_ADDR(0x04)
#define PIN_CCIRHS_REG              PINMAP_REG_ADDR(0x08)
#define PIN_CCIRVS_REG              PINMAP_REG_ADDR(0x0C)
#define PIN_CCIRRST_REG             PINMAP_REG_ADDR(0x10)
#define PIN_CCIRPD_REG              PINMAP_REG_ADDR(0x14)
#define PIN_CCIRD0_REG              PINMAP_REG_ADDR(0x18)
#define PIN_CCIRD1_REG              PINMAP_REG_ADDR(0x1C)
#define PIN_CCIRD2_REG              PINMAP_REG_ADDR(0x20)
#define PIN_CCIRD3_REG              PINMAP_REG_ADDR(0x24)
#define PIN_CCIRD4_REG              PINMAP_REG_ADDR(0x28)
#define PIN_CCIRD5_REG              PINMAP_REG_ADDR(0x2C)
#define PIN_CCIRD6_REG              PINMAP_REG_ADDR(0x30)
#define PIN_CCIRD7_REG              PINMAP_REG_ADDR(0x34)
#define PIN_CCIRMCK_REG             PINMAP_REG_ADDR(0x38)
#define PIN_SCL_REG                 PINMAP_REG_ADDR(0x3C)
#define PIN_SDA_REG                 PINMAP_REG_ADDR(0x40)
#define PIN_U0RXD_REG               PINMAP_REG_ADDR(0x44)
#define PIN_U0TXD_REG               PINMAP_REG_ADDR(0x48)
#define PIN_U0CTS_REG               PINMAP_REG_ADDR(0x4C)
#define PIN_U0RTS_REG               PINMAP_REG_ADDR(0x50)
#define PIN_U1RXD_REG               PINMAP_REG_ADDR(0x54)
#define PIN_U1TXD_REG               PINMAP_REG_ADDR(0x58)
#define PIN_SPI_DI_REG              PINMAP_REG_ADDR(0x5C)
#define PIN_SPI_DO_REG              PINMAP_REG_ADDR(0x60)
#define PIN_SPI_CLK_REG             PINMAP_REG_ADDR(0x64)
#define PIN_SPI_CS0_REG             PINMAP_REG_ADDR(0x68)
#define PIN_IISDO_REG               PINMAP_REG_ADDR(0x6C)
#define PIN_IISDI_REG               PINMAP_REG_ADDR(0x70)
#define PIN_IISCLK_REG              PINMAP_REG_ADDR(0x74)
#define PIN_IISLRCK_REG             PINMAP_REG_ADDR(0x78)
#define PIN_PBINT_REG               PINMAP_REG_ADDR(0x7C)
#define PIN_KEYOUT0_REG             PINMAP_REG_ADDR(0x80)
#define PIN_KEYOUT1_REG             PINMAP_REG_ADDR(0x84)
#define PIN_KEYOUT2_REG             PINMAP_REG_ADDR(0x88)
#define PIN_KEYOUT3_REG             PINMAP_REG_ADDR(0x8C)
#define PIN_KEYOUT4_REG             PINMAP_REG_ADDR(0x90)
#define PIN_KEYOUT5_REG             PINMAP_REG_ADDR(0x94)
#define PIN_KEYOUT6_REG             PINMAP_REG_ADDR(0x98)
#define PIN_KEYOUT7_REG             PINMAP_REG_ADDR(0x9C)
#define PIN_KEYIN0_REG              PINMAP_REG_ADDR(0xA0)
#define PIN_KEYIN1_REG              PINMAP_REG_ADDR(0xA4)
#define PIN_KEYIN2_REG              PINMAP_REG_ADDR(0xA8)
#define PIN_KEYIN3_REG              PINMAP_REG_ADDR(0xAC)
#define PIN_KEYIN4_REG              PINMAP_REG_ADDR(0xB0)
#define PIN_KEYIN5_REG              PINMAP_REG_ADDR(0xB4)
#define PIN_KEYIN6_REG              PINMAP_REG_ADDR(0xB8)
#define PIN_KEYIN7_REG              PINMAP_REG_ADDR(0xBC)
#define PIN_MTDO_REG                PINMAP_REG_ADDR(0xC0)
#define PIN_MTDI_REG                PINMAP_REG_ADDR(0xC4)
#define PIN_MTCK_REG                PINMAP_REG_ADDR(0xC8)
#define PIN_MTMS_REG                PINMAP_REG_ADDR(0xCC)
#define PIN_MTRSTN_REG              PINMAP_REG_ADDR(0xD0)
#define PIN_SIM0_CLK_REG            PINMAP_REG_ADDR(0xD4)
#define PIN_SIM0_DA_REG             PINMAP_REG_ADDR(0xD8)
#define PIN_SIM0_RST_REG            PINMAP_REG_ADDR(0xDC)
#define PIN_SIM1_CLK_REG            PINMAP_REG_ADDR(0xE0)
#define PIN_SIM1_DA_REG             PINMAP_REG_ADDR(0xE4)
#define PIN_SIM1_RST_REG            PINMAP_REG_ADDR(0xE8)
#define PIN_SD_CLK0_REG             PINMAP_REG_ADDR(0xEC)
#define PIN_SD_CMD_REG              PINMAP_REG_ADDR(0xF0)
#define PIN_SD_D0_REG               PINMAP_REG_ADDR(0xF4)
#define PIN_SD_D1_REG               PINMAP_REG_ADDR(0xF8)
#define PIN_SD_D2_REG               PINMAP_REG_ADDR(0xFC)
#define PIN_SD_D3_REG               PINMAP_REG_ADDR(0x100)
#define PIN_SD_CLK1_REG             PINMAP_REG_ADDR(0x104)
#define PIN_EMA0_REG                PINMAP_REG_ADDR(0x108)
#define PIN_EMA1_REG                PINMAP_REG_ADDR(0x10C)
#define PIN_EMA2_REG                PINMAP_REG_ADDR(0x110)
#define PIN_EMA3_REG                PINMAP_REG_ADDR(0x114)
#define PIN_EMA4_REG                PINMAP_REG_ADDR(0x118)
#define PIN_EMA5_REG                PINMAP_REG_ADDR(0x11C)
#define PIN_EMA6_REG                PINMAP_REG_ADDR(0x120)
#define PIN_EMA7_REG                PINMAP_REG_ADDR(0x124)
#define PIN_EMA8_REG                PINMAP_REG_ADDR(0x128)
#define PIN_EMA9_REG                PINMAP_REG_ADDR(0x12C)
#define PIN_EMA10_REG               PINMAP_REG_ADDR(0x130)
#define PIN_EMA11_REG               PINMAP_REG_ADDR(0x134)
#define PIN_EMA12_REG               PINMAP_REG_ADDR(0x138)
#define PIN_EMA13_REG               PINMAP_REG_ADDR(0x13C)
#define PIN_EMA14_REG               PINMAP_REG_ADDR(0x140)
#define PIN_EMA15_REG               PINMAP_REG_ADDR(0x144)
#define PIN_EMA16_REG               PINMAP_REG_ADDR(0x148)
#define PIN_EMA17_REG               PINMAP_REG_ADDR(0x14c)
#define PIN_EMA18_REG               PINMAP_REG_ADDR(0x150)
#define PIN_EMA19_REG               PINMAP_REG_ADDR(0x154)
#define PIN_EMA20_REG               PINMAP_REG_ADDR(0x158)
#define PIN_EMA21_REG               PINMAP_REG_ADDR(0x15c)
#define PIN_EMA22_REG               PINMAP_REG_ADDR(0x160)
#define PIN_EMA23_REG               PINMAP_REG_ADDR(0x164)
#define PIN_EMA24_REG               PINMAP_REG_ADDR(0x168)
#define PIN_EMCRE_REG               PINMAP_REG_ADDR(0x16c)
#define PIN_EMD0_REG                PINMAP_REG_ADDR(0x170)
#define PIN_EMD1_REG                PINMAP_REG_ADDR(0x174)
#define PIN_EMD2_REG                PINMAP_REG_ADDR(0x178)
#define PIN_EMD3_REG                PINMAP_REG_ADDR(0x17c)
#define PIN_EMD4_REG                PINMAP_REG_ADDR(0x180)
#define PIN_EMD5_REG                PINMAP_REG_ADDR(0x184)
#define PIN_EMD6_REG                PINMAP_REG_ADDR(0x188)
#define PIN_EMD7_REG                PINMAP_REG_ADDR(0x18c)
#define PIN_EMD8_REG                PINMAP_REG_ADDR(0x190)
#define PIN_EMD9_REG                PINMAP_REG_ADDR(0x194)
#define PIN_EMD10_REG               PINMAP_REG_ADDR(0x198)
#define PIN_EMD11_REG               PINMAP_REG_ADDR(0x19c)
#define PIN_EMD12_REG               PINMAP_REG_ADDR(0x1a0)
#define PIN_EMD13_REG               PINMAP_REG_ADDR(0x1a4)
#define PIN_EMD14_REG               PINMAP_REG_ADDR(0x1A8)
#define PIN_EMD15_REG               PINMAP_REG_ADDR(0x1Ac)
#define PIN_EMRASN_REG              PINMAP_REG_ADDR(0x1b0)
#define PIN_EMCASN_REG              PINMAP_REG_ADDR(0x1b4)
#define PIN_EMWEN_REG               PINMAP_REG_ADDR(0x1B8)
#define PIN_EMCLKDP_REG             PINMAP_REG_ADDR(0x1Bc)
#define PIN_EMDQM0_REG              PINMAP_REG_ADDR(0x1C0)
#define PIN_EMDQM1_REG              PINMAP_REG_ADDR(0x1C4)
#define PIN_EMCSN0_REG              PINMAP_REG_ADDR(0x1C8)
#define PIN_EMCSN1_REG              PINMAP_REG_ADDR(0x1CC)
#define PIN_EMCSN2_REG              PINMAP_REG_ADDR(0x1D0)
#define PIN_EMCKE_REG               PINMAP_REG_ADDR(0x1D4)
#define PIN_EWAIT_REG               PINMAP_REG_ADDR(0x1D8)
#define PIN_EMRSTN_REG              PINMAP_REG_ADDR(0x1DC)
#define PIN_NFWPN_REG               PINMAP_REG_ADDR(0x1E0)
#define PIN_NFRB_REG                PINMAP_REG_ADDR(0x1E4)
#define PIN_NFCLE_REG               PINMAP_REG_ADDR(0x1E8)
#define PIN_NFALE_REG               PINMAP_REG_ADDR(0x1EC)
#define PIN_NFCEN_REG               PINMAP_REG_ADDR(0x1F0)
#define PIN_NFWEN_REG               PINMAP_REG_ADDR(0x1F4)
#define PIN_NFREN_REG               PINMAP_REG_ADDR(0x1F8)
#define PIN_NFD0_REG                PINMAP_REG_ADDR(0x1FC)
#define PIN_NFD1_REG                PINMAP_REG_ADDR(0x200)
#define PIN_NFD2_REG                PINMAP_REG_ADDR(0x204)
#define PIN_NFD3_REG                PINMAP_REG_ADDR(0x208)
#define PIN_NFD4_REG                PINMAP_REG_ADDR(0x20C)
#define PIN_NFD5_REG                PINMAP_REG_ADDR(0x210)
#define PIN_NFD6_REG                PINMAP_REG_ADDR(0x214)
#define PIN_NFD7_REG                PINMAP_REG_ADDR(0x218)
#define PIN_NFD8_REG                PINMAP_REG_ADDR(0x21C)
#define PIN_NFD9_REG                PINMAP_REG_ADDR(0x220)
#define PIN_NFD10_REG               PINMAP_REG_ADDR(0x224)
#define PIN_NFD11_REG               PINMAP_REG_ADDR(0x228)
#define PIN_NFD12_REG               PINMAP_REG_ADDR(0x22C)
#define PIN_NFD13_REG               PINMAP_REG_ADDR(0x230)
#define PIN_NFD14_REG               PINMAP_REG_ADDR(0x234)
#define PIN_NFD15_REG               PINMAP_REG_ADDR(0x238)
#define PIN_LCMD16_REG              PINMAP_REG_ADDR(0x23C)
#define PIN_LCMD17_REG              PINMAP_REG_ADDR(0x240)
#define PIN_LCMCD_REG               PINMAP_REG_ADDR(0x244)
#define PIN_LCMCSN0_REG             PINMAP_REG_ADDR(0x248)
#define PIN_LCMCSN1_REG             PINMAP_REG_ADDR(0x24C)
#define PIN_LCMFMARK_REG            PINMAP_REG_ADDR(0x250)
#define PIN_LCMRSTN_REG             PINMAP_REG_ADDR(0x254)
#define PIN_XTLEN_REG               PINMAP_REG_ADDR(0x258)
#define PIN_RFCTL12_REG             PINMAP_REG_ADDR(0x25C)
#define PIN_RFCTL13_REG             PINMAP_REG_ADDR(0x260)
#define PIN_RFCTL14_REG             PINMAP_REG_ADDR(0x264)
#define PIN_RFCTL15_REG             PINMAP_REG_ADDR(0x268)
#define PIN_RFCTL2_REG              PINMAP_REG_ADDR(0x26c)
#define PIN_RFCTL8_REG              PINMAP_REG_ADDR(0x270)
#define PIN_RFSDA_REG               PINMAP_REG_ADDR(0x274)
#define PIN_RFSCK_REG               PINMAP_REG_ADDR(0x278)
#define PIN_RFSEN_REG               PINMAP_REG_ADDR(0x27c)
#define PIN_AUXCLK0_REG             PINMAP_REG_ADDR(0x280)
#define PIN_TP_XL_REG               PINMAP_REG_ADDR(0x284)
#define PIN_TP_XR_REG               PINMAP_REG_ADDR(0x288)
#define PIN_TP_YU_REG               PINMAP_REG_ADDR(0x28c)
#define PIN_TP_YD_REG               PINMAP_REG_ADDR(0x290)
#define PIN_GPO58_REG               PINMAP_REG_ADDR(0x294)
#define PIN_GPO48_REG               PINMAP_REG_ADDR(0x25c)
#define PIN_BT_XTLEN_REG            PINMAP_REG_ADDR(0x298)
#define PIN_GPI0_REG                PINMAP_REG_ADDR(0x29c)
#define PIN_LED_R_REG               PINMAP_REG_ADDR(0x2a0)
#define PIN_LED_G_REG               PINMAP_REG_ADDR(0x2a4)
#define PIN_LED_B_REG               PINMAP_REG_ADDR(0x2a8)
#define PIN_LED_W_REG               PINMAP_REG_ADDR(0x2ac)
#define PIN_SIM2_CLK_REG            PINMAP_REG_ADDR(0x2b0)
#define PIN_SIM2_DA_REG             PINMAP_REG_ADDR(0x2b4)
#define PIN_SIM2_RST_REG            PINMAP_REG_ADDR(0x2b8)
#define PIN_SIM3_CLK_REG            PINMAP_REG_ADDR(0x2bc)
#define PIN_SIM3_DA_REG             PINMAP_REG_ADDR(0x2c0)
#define PIN_SIM3_RST_REG            PINMAP_REG_ADDR(0x2c4)

#define CPC_SEL_REG0                    (PIN_CTL_BASE + 0x0000)
#define CPC_SEL_REG1                    (PIN_CTL_BASE + 0x0004)
#define CPC_SEL_REG2                    (PIN_CTL_BASE + 0x0008)
#define CPC_SEL_REG3                    (PIN_CTL_BASE + 0x000C)
#define CPC_SEL_REG4                    (PIN_CTL_BASE + 0x0010)
#define CPC_SEL_REG5                    (PIN_CTL_BASE + 0x0014)
#define CPC_SEL_REG6                    (PIN_CTL_BASE + 0x0018)
#define CPC_SEL_REG7                    (PIN_CTL_BASE + 0x001C)
#define CPC_SEL_REG8                    (PIN_CTL_BASE + 0x0020)
#define CPC_SEL_REG9                    (PIN_CTL_BASE + 0x0024)
#define CPC_SEL_REG10                   (PIN_CTL_BASE + 0x0028)
#define CPC_SEL_REG11                   (PIN_CTL_BASE + 0x002C)
#define CPC_SEL_REG12                   (PIN_CTL_BASE + 0x0030)
#define CPC_SEL_REG13                   (PIN_CTL_BASE + 0x0034)
#define CPC_SEL_REG14                   (PIN_CTL_BASE + 0x0038)
#define CPC_SEL_REG15                   (PIN_CTL_BASE + 0x003C)
#define CPC_SEL_REG16                   (PIN_CTL_BASE + 0x0040)
#define CPC_SEL_REG17                   (PIN_CTL_BASE + 0x0044)
#define CPC_SEL_REG18                   (PIN_CTL_BASE + 0x0048)
#define CPC_SEL_REG19                   (PIN_CTL_BASE + 0x004C)
#define CPC_SEL_REG22                   (PIN_CTL_BASE + 0x0058)
#define CPC_SEL_REG23                   (PIN_CTL_BASE + 0x005C)
#define CPC_SEL_REG24                   (PIN_CTL_BASE + 0x0060)


#define CPC_DRV_REG0                    (PIN_CTL_BASE + 0x0064)
#define CPC_DRV_REG1                    (PIN_CTL_BASE + 0x0068)
#define CPC_DRV_REG2                    (PIN_CTL_BASE + 0x006C)
#define CPC_DRV_REG3                    (PIN_CTL_BASE + 0x0070)
#define CPC_DRV_REG4                    (PIN_CTL_BASE + 0x0074)
#define CPC_DRV_REG5                    (PIN_CTL_BASE + 0x0078)
#define CPC_DRV_REG6                    (PIN_CTL_BASE + 0x007C)
#define CPC_DRV_REG7                    (PIN_CTL_BASE + 0x0080)
#define CPC_DRV_REG8                    (PIN_CTL_BASE + 0x0084)
#define CPC_DRV_REG9                    (PIN_CTL_BASE + 0x0088)
#define CPC_DRV_REG10                   (PIN_CTL_BASE + 0x008C)
#define CPC_DRV_REG11                   (PIN_CTL_BASE + 0x0090)
#define CPC_DRV_REG12                   (PIN_CTL_BASE + 0x0094)
#define CPC_DRV_REG13                   (PIN_CTL_BASE + 0x0098)
#define CPC_DRV_REG14                   (PIN_CTL_BASE + 0x009C)
#define CPC_DRV_REG15                   (PIN_CTL_BASE + 0x00A0)
#define CPC_DRV_REG16                   (PIN_CTL_BASE + 0x00A4)
#define CPC_DRV_REG17                   (PIN_CTL_BASE + 0x00A8)
#define CPC_DRV_REG18                   (PIN_CTL_BASE + 0x00AC)
#define CPC_DRV_REG19                   (PIN_CTL_BASE + 0x00B0)
#define CPC_WPU_REG0                    (PIN_CTL_BASE + 0x00C8)
#define CPC_WPU_REG1                    (PIN_CTL_BASE + 0x00CC)
#define CPC_WPU_REG2                    (PIN_CTL_BASE + 0x00D0)
#define CPC_WPU_REG3                    (PIN_CTL_BASE + 0x00D4)
#define CPC_WPU_REG4                    (PIN_CTL_BASE + 0x00D8)
#define CPC_WPU_REG5                    (PIN_CTL_BASE + 0x00DC)
#define CPC_WPU_REG6                    (PIN_CTL_BASE + 0x00E0)
#define CPC_WPU_REG7                    (PIN_CTL_BASE + 0x00E4)
#define CPC_WPU_REG8                    (PIN_CTL_BASE + 0x00E8)
#define CPC_WPU_REG9                    (PIN_CTL_BASE + 0x00EC)
#define CPC_SIM_WPU                     (PIN_CTL_BASE + 0x00EC)
#define CPC_CTL_REG0                    (PIN_CTL_BASE + 0x00F8)

#define CPC_WPD_REG0                    (PIN_CTL_BASE + 0x0104)
#define CPC_WPD_REG1                    (PIN_CTL_BASE + 0x0108)
#define CPC_WPD_REG2                    (PIN_CTL_BASE + 0x010C)
#define CPC_WPD_REG3                    (PIN_CTL_BASE + 0x0110)
#define CPC_WPD_REG4                    (PIN_CTL_BASE + 0x0114)
#define CPC_WPD_REG5                    (PIN_CTL_BASE + 0x0118)
#define CPC_WPD_REG6                    (PIN_CTL_BASE + 0x011C)
#define CPC_WPD_REG7                    (PIN_CTL_BASE + 0x0120)
#define CPC_WPD_REG8                    (PIN_CTL_BASE + 0x0124)
#define CPC_WPD_REG9                    (PIN_CTL_BASE + 0x0128)
#define CPC_WPD_REG10                   (PIN_CTL_BASE + 0x012C)
#define CPC_WPD_REG11                   (PIN_CTL_BASE + 0x0130)
#define CPC_WPD_REG12                   (PIN_CTL_BASE + 0x0134)


/* Pinmap ctrl register Bit field value
------------------------------------------------------------------------------------------------------------
|                |               |                 |            |             |             |              |
| Reserved[31:8] | Func Sel[7:6] | Drv Strght[5:4] | Pull Up[3] | Pull Down[2]| Input EN[1] | Output EN[0] |
|                |               |                 |            |             |             |              |
------------------------------------------------------------------------------------------------------------
*/

#define PIN_Z_EN                0x000           // High-Z in sleep mode
#define PIN_O_EN                0x001           // Output enable in sleep mode
#define PIN_I_EN                0x002           // Input enable in sleep mode
#define PIN_SPD_EN              0x004           // Pull down enable in sleep mode
#define PIN_SPU_EN              0x008           // Pull up enable in sleep mode
#define PIN_SPX_EN              0x000           // Don't pull down or up  in sleep mode

#define PIN_FUNC_DEF            0x000
#define PIN_FUNC_1              0x010
#define PIN_FUNC_2              0x020
#define PIN_FUNC_3              0x030

#define PIN_FPD_EN              0x040            // Pull down enable in Function mode
#define PIN_FPU_EN              0x080            // Pull up enable in Function mode
#define PIN_FPX_EN              0x000            // Don't pull down or up  in Function mode

#define PIN_DS_0                0x000           // Driver strength level 0
#define PIN_DS_1                0x100           // Driver strength level 1
#define PIN_DS_2                0x200           // Driver strength level 2
#define PIN_DS_3                0x300           // Driver strength level 3





/* Pinmap control register bit field value structure*/
typedef union _pinmap_ctl_reg_u
{
    struct pinmap_ctl_reg_tag
    {
        volatile int reserved       :22;
        volatile int drv_strght     :2;
        volatile int pull_up_en     :1;
        volatile int pull_down_en   :1;
        volatile int func_sel       :2;
        volatile int ds_pull_up_en      :1;
        volatile int ds_pull_down_en    :1;
        volatile int input_en       :1;
        volatile int output_en      :1;
    } mBits;
    volatile int dwValue;

} PINMAP_CTL_REG_U;

/* Pin control register 0 bit field value structure*/
typedef union _pin_ctl_reg0_u
{
    struct pin_ctl_reg0_tag
    {
        volatile int reserved       :24;
        volatile int keyin7_wpus  :1;
        volatile int keyin6_wpus  :1;
        volatile int keyin5_wpus  :1;
        volatile int keyin_wpus   :1;
        volatile int nfrb_wpus      :1;
        volatile int i2c_wpus       :1;
        volatile int simda1_wpus    :1;
        volatile int simda0_wpus    :1;
    } mBits;
    volatile int dwValue;

} PIN_CTL_REG0_U;


#endif  //_SC6600L_REG_PINMAP_H_

