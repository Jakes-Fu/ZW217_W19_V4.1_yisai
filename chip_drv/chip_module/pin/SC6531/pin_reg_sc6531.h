/******************************************************************************
 ** File Name:    pin_reg_sc6531.h                                           *
 ** Author:       Mingwei.Zhang                                               *
 ** DATE:         05/13/2010                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 05/13/2010    Mingwei.Zhang   Create.                                     *
 ******************************************************************************/
#ifndef _PIN_REG_SC6531_H_
#define _PIN_REG_SC6531_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/

//PIN_CTL_BASE  0x8C000000

#define PINMAP_REG_BASE     PIN_CTL_BASE
#define ANA_PINMAP_REG_BASE ANA_PIN_CTL_BASE
#define REG_ADDR(_x_)       (PINMAP_REG_BASE + _x_)
#define ANA_REG_ADDR(_x_)   (ANA_PINMAP_REG_BASE + _x_)

/*----------Digital Die Pin Control Register----------*/

#define    PIN_CTRL_REG             REG_ADDR(0x0000)
#define    PIN_ADI_D_REG            REG_ADDR(0x0004)
#define    PIN_ADI_SCLK_REG         REG_ADDR(0x0008)
#define    PIN_ADI_SYNC_REG         REG_ADDR(0x000C)
#define    PIN_ANA_IRQ_REG          REG_ADDR(0x0010)
#define    PIN_AUD_ADD0_REG         REG_ADDR(0x0014)
#define    PIN_AUD_ADSYNC_REG       REG_ADDR(0x0018)
#define    PIN_AUD_DAD0_REG         REG_ADDR(0x001C)
#define    PIN_AUD_DAD1_REG         REG_ADDR(0x0020)
#define    PIN_AUD_DASYNC_REG       REG_ADDR(0x0024)
#define    PIN_CLK_32K_REG          REG_ADDR(0x0028)
#define    PIN_CLK_AUX_REG          REG_ADDR(0x002C)
#define    PIN_CLK_ITF_REG          REG_ADDR(0x0030)
#define    PIN_CHIP_SLEEP_REG       REG_ADDR(0x0034)
#define    PIN_EXT_RST_B_REG        REG_ADDR(0x0038)
#define    PIN_FM_RXID0_REG         REG_ADDR(0x003C)
#define    PIN_FM_RXID1_REG         REG_ADDR(0x0040)
#define    PIN_FM_RXQD0_REG         REG_ADDR(0x0044)
#define    PIN_FM_RXQD1_REG         REG_ADDR(0x0048)
#define    PIN_PTEST_REG            REG_ADDR(0x004C)
#define    PIN_RFSCK_REG            REG_ADDR(0x0050)
#define    PIN_RFSDA_REG            REG_ADDR(0x0054)
#define    PIN_RFSEN_REG            REG_ADDR(0x0058)
#define    PIN_RXID0_REG            REG_ADDR(0x005C)
#define    PIN_RXID1_REG            REG_ADDR(0x0060)
#define    PIN_RXQD0_REG            REG_ADDR(0x0064)
#define    PIN_RXQD1_REG            REG_ADDR(0x0068)
#define    PIN_XTL_BUF_EN_REG       REG_ADDR(0x006C)
#define    PIN_ANA_FIQ_REG          REG_ADDR(0x0070)
#define    PIN_CAM_PD_REG           REG_ADDR(0x0074)
#define    PIN_CAM_RSTN_REG         REG_ADDR(0x0078)
#define    PIN_CCIRCK_REG           REG_ADDR(0x007C)
#define    PIN_CCIRD0_REG           REG_ADDR(0x0080)
#define    PIN_CCIRD1_REG           REG_ADDR(0x0084)
#define    PIN_CCIRD2_REG           REG_ADDR(0x0088)
#define    PIN_CCIRD3_REG           REG_ADDR(0x008C)
#define    PIN_CCIRD4_REG           REG_ADDR(0x0090)
#define    PIN_CCIRD5_REG           REG_ADDR(0x0094)
#define    PIN_CCIRD6_REG           REG_ADDR(0x0098)
#define    PIN_CCIRD7_REG           REG_ADDR(0x009C)
#define    PIN_CCIRHS_REG           REG_ADDR(0x00A0)
#define    PIN_CCIRMCK_REG          REG_ADDR(0x00A4)
#define    PIN_CCIRVS_REG           REG_ADDR(0x00A8)
#define    PIN_EMCLK_REG            REG_ADDR(0x00AC)
#define    PIN_EMCLKN_REG           REG_ADDR(0x00B0)
#define    PIN_EMADVN_REG           REG_ADDR(0x00B4)
#define    PIN_EMA16_REG            REG_ADDR(0x00B8)
#define    PIN_EMA17_REG            REG_ADDR(0x00BC)
#define    PIN_EMA18_REG            REG_ADDR(0x00C0)
#define    PIN_EMA19_REG            REG_ADDR(0x00C4)
#define    PIN_EMA20_REG            REG_ADDR(0x00C8)
#define    PIN_EMA21_REG            REG_ADDR(0x00CC)
#define    PIN_EMA22_REG            REG_ADDR(0x00D0)
#define    PIN_EMA23_REG            REG_ADDR(0x00D4)
#define    PIN_EMCEN1_REG           REG_ADDR(0x00D8)
#define    PIN_EMCRE_REG            REG_ADDR(0x00DC)
#define    PIN_EMDQS0_REG           REG_ADDR(0x00E0)
#define    PIN_EMDQS1_REG           REG_ADDR(0x00E4)
#define    PIN_EMD0_REG             REG_ADDR(0x00E8)
#define    PIN_EMD1_REG             REG_ADDR(0x00EC)
#define    PIN_EMD2_REG             REG_ADDR(0x00F0)
#define    PIN_EMD3_REG             REG_ADDR(0x00F4)
#define    PIN_EMD4_REG             REG_ADDR(0x00F8)
#define    PIN_EMD5_REG             REG_ADDR(0x00FC)
#define    PIN_EMD6_REG             REG_ADDR(0x0100)
#define    PIN_EMD7_REG             REG_ADDR(0x0104)
#define    PIN_EMD8_REG             REG_ADDR(0x0108)
#define    PIN_EMD9_REG             REG_ADDR(0x010C)
#define    PIN_EMD10_REG            REG_ADDR(0x0110)
#define    PIN_EMD11_REG            REG_ADDR(0x0114)
#define    PIN_EMD12_REG            REG_ADDR(0x0118)
#define    PIN_EMD13_REG            REG_ADDR(0x011C)
#define    PIN_EMD14_REG            REG_ADDR(0x0120)
#define    PIN_EMD15_REG            REG_ADDR(0x0124)
#define    PIN_EMLBN_REG            REG_ADDR(0x0128)
#define    PIN_EMOEN_REG            REG_ADDR(0x012C)
#define    PIN_EMSMP_REG            REG_ADDR(0x0130)
#define    PIN_EMUBN_REG            REG_ADDR(0x0134)
#define    PIN_EMWAIT_REG           REG_ADDR(0x0138)
#define    PIN_EMWEN_REG            REG_ADDR(0x013C)
#define    PIN_U0CTS_REG            REG_ADDR(0x0140)
#define    PIN_U0RTS_REG            REG_ADDR(0x0144)
#define    PIN_U0RXD_REG            REG_ADDR(0x0148)
#define    PIN_U0TXD_REG            REG_ADDR(0x014C)
#define    PIN_ESMCLK_REG           REG_ADDR(0x0150)
#define    PIN_ESMCSN0_REG          REG_ADDR(0x0154)
#define    PIN_ESMCSN1_REG          REG_ADDR(0x0158)
#define    PIN_ESMD0_REG            REG_ADDR(0x015C)
#define    PIN_ESMD1_REG            REG_ADDR(0x0160)
#define    PIN_ESMD2_REG            REG_ADDR(0x0164)
#define    PIN_ESMD3_REG            REG_ADDR(0x0168)
#define    PIN_ESMSMP_REG           REG_ADDR(0x016C)
#define    PIN_EXTINT0_REG          REG_ADDR(0x0170)
#define    PIN_SD_CLK1_REG          REG_ADDR(0x0174)
#define    PIN_GPIO_0_REG           REG_ADDR(0x0178)
#define    PIN_GPIO_1_REG           REG_ADDR(0x017C)
#define    PIN_GPIO_2_REG           REG_ADDR(0x0180)
#define    PIN_GPIO_3_REG           REG_ADDR(0x0184)
#define    PIN_GPIO_63_REG          REG_ADDR(0x0188)
#define    PIN_IISCLK_REG           REG_ADDR(0x018C)
#define    PIN_IISDI_REG            REG_ADDR(0x0190)
#define    PIN_IISDO_REG            REG_ADDR(0x0194)
#define    PIN_IISLRCK_REG          REG_ADDR(0x0198)
#define    PIN_IISMCK_REG           REG_ADDR(0x019C)
#define    PIN_KEYIN0_REG           REG_ADDR(0x01A0)
#define    PIN_KEYIN1_REG           REG_ADDR(0x01A4)
#define    PIN_KEYIN2_REG           REG_ADDR(0x01A8)
#define    PIN_KEYIN3_REG           REG_ADDR(0x01AC)
#define    PIN_KEYIN4_REG           REG_ADDR(0x01B0)
#define    PIN_KEYIN5_REG           REG_ADDR(0x01B4)
#define    PIN_KEYIN6_REG           REG_ADDR(0x01B8)
#define    PIN_KEYIN7_REG           REG_ADDR(0x01BC)
#define    PIN_KEYOUT0_REG          REG_ADDR(0x01C0)
#define    PIN_KEYOUT1_REG          REG_ADDR(0x01C4)
#define    PIN_KEYOUT2_REG          REG_ADDR(0x01C8)
#define    PIN_KEYOUT3_REG          REG_ADDR(0x01CC)
#define    PIN_KEYOUT4_REG          REG_ADDR(0x01D0)
#define    PIN_KEYOUT5_REG          REG_ADDR(0x01D4)
#define    PIN_KEYOUT6_REG          REG_ADDR(0x01D8)
#define    PIN_FMARK_REG            REG_ADDR(0x01DC)
#define    PIN_LCMCD_REG            REG_ADDR(0x01E0)
#define    PIN_LCMCS0_REG           REG_ADDR(0x01E4)
#define    PIN_LCMCS1_REG           REG_ADDR(0x01E8)
#define    PIN_LCMD0_REG            REG_ADDR(0x01EC)
#define    PIN_LCMD1_REG            REG_ADDR(0x01F0)
#define    PIN_LCMD2_REG            REG_ADDR(0x01F4)
#define    PIN_LCMD3_REG            REG_ADDR(0x01F8)
#define    PIN_LCMD4_REG            REG_ADDR(0x01FC)
#define    PIN_LCMD5_REG            REG_ADDR(0x0200)
#define    PIN_LCMD6_REG            REG_ADDR(0x0204)
#define    PIN_LCMD7_REG            REG_ADDR(0x0208)
#define    PIN_LCMD8_REG            REG_ADDR(0x020C)
#define    PIN_LCMRD_REG            REG_ADDR(0x0210)
#define    PIN_LCMRSTN_REG          REG_ADDR(0x0214)
#define    PIN_LCMWR_REG            REG_ADDR(0x0218)
#define    PIN_GPIO_70_REG          REG_ADDR(0x021C)
#define    PIN_GPIO_71_REG          REG_ADDR(0x0220)
#define    PIN_GPIO_72_REG          REG_ADDR(0x0224)
#define    PIN_SPI1_0_CS0_REG       REG_ADDR(0x0228)
#define    PIN_SIMRST0_REG          REG_ADDR(0x022C)
#define    PIN_SIMRST1_REG          REG_ADDR(0x0230)
#define    PIN_SIMRST2_REG          REG_ADDR(0x0234)
#define    PIN_SPI0_3_DO_REG        REG_ADDR(0x0238)
#define    PIN_SPI0_3_DI_REG        REG_ADDR(0x023C)
#define    PIN_MTCK_REG             REG_ADDR(0x0240)
#define    PIN_MTDI_REG             REG_ADDR(0x0244)
#define    PIN_MTDO_REG             REG_ADDR(0x0248)
#define    PIN_MTMS_REG             REG_ADDR(0x024C)
#define    PIN_MTRSTN_REG           REG_ADDR(0x0250)
#define    PIN_PWM_REG              REG_ADDR(0x0254)
#define    PIN_RF_VBS0_REG          REG_ADDR(0x0258)
#define    PIN_RF_PAEN_REG          REG_ADDR(0x025C)
#define    PIN_RF_RSV_REG           REG_ADDR(0x0260)
#define    PIN_RF_VBS1_REG          REG_ADDR(0x0264)
#define    PIN_RF_VBS2_REG          REG_ADDR(0x0268)
#define    PIN_RF_VBS3_REG          REG_ADDR(0x026C)
#define    PIN_SCL0_0_REG           REG_ADDR(0x0270)
#define    PIN_SDA0_0_REG           REG_ADDR(0x0274)
#define    PIN_SD_CLK_REG           REG_ADDR(0x0278)
#define    PIN_SD_CMD_REG           REG_ADDR(0x027C)
#define    PIN_SD_D0_REG            REG_ADDR(0x0280)
#define    PIN_SD_D1_REG            REG_ADDR(0x0284)
#define    PIN_SD_D2_REG            REG_ADDR(0x0288)
#define    PIN_SD_D3_REG            REG_ADDR(0x028C)
#define    PIN_SIMCLK0_REG          REG_ADDR(0x0290)
#define    PIN_SIMCLK1_REG          REG_ADDR(0x0294)
#define    PIN_SIMCLK2_REG          REG_ADDR(0x0298)
#define    PIN_SIMDA0_REG           REG_ADDR(0x029C)
#define    PIN_SIMDA1_REG           REG_ADDR(0x02A0)
#define    PIN_SIMDA2_REG           REG_ADDR(0x02A4)
#define    PIN_SPI0_3_CLK_REG       REG_ADDR(0x02A8)
#define    PIN_SPI0_3_CS0_REG       REG_ADDR(0x02AC)

#define    PIN_ACLK_32K_REG         REG_ADDR(0x0028)
#define    PIN_ACLK_AUX_REG         REG_ADDR(0x002C)
#define    PIN_ACLK_ITF_REG         REG_ADDR(0x0030)
#define    PIN_ACHIP_SLEEP_REG      REG_ADDR(0x0034)
#define    PIN_AEXT_RST_B_REG       REG_ADDR(0x0038)
#define    PIN_AFM_RXID0_REG        REG_ADDR(0x003C)
#define    PIN_AFM_RXID1_REG        REG_ADDR(0x0040)
#define    PIN_AFM_RXQD0_REG        REG_ADDR(0x0044)
#define    PIN_AFM_RXQD1_REG        REG_ADDR(0x0048)
#define    PIN_APTEST_REG           REG_ADDR(0x004C)
#define    PIN_ARFSCK_REG           REG_ADDR(0x0050)
#define    PIN_ARFSDA_REG           REG_ADDR(0x0054)
#define    PIN_ARFSEN_REG           REG_ADDR(0x0058)
#define    PIN_ARXID0_REG           REG_ADDR(0x005C)
#define    PIN_ARXID1_REG           REG_ADDR(0x0060)
#define    PIN_ARXQD0_REG           REG_ADDR(0x0064)
#define    PIN_ARXQD1_REG           REG_ADDR(0x0068)
#define    PIN_AXTL_BUF_EN_REG      REG_ADDR(0x006C)

//temp
 #define	PIN_SPI_CLK_REG	PIN_SD_CLK_REG	
 #define	PIN_SPI_DI_REG	PIN_SD_D0_REG
 #define	PIN_SPI_DO_REG	PIN_SD_CMD_REG
 #define	PIN_SPI_CS0_REG	PIN_SPI0_0_CS1_REG
//temp end

/*----------Analog Die Pin Control Register----------*/

#define   ANA_PIN_CTRL_REG       ANA_REG_ADDR(0x0000)  
#define   ANA_PIN_RXID0_REG          ANA_REG_ADDR(0x0004)  
#define   ANA_PIN_RXID1_REG          ANA_REG_ADDR(0x0008)  
#define   ANA_PIN_RXQD0_REG          ANA_REG_ADDR(0x000C)  
#define   ANA_PIN_RXQD1_REG          ANA_REG_ADDR(0x0010)  
#define   ANA_PIN_AUD_DASYNC_REG     ANA_REG_ADDR(0x0014)  
#define   ANA_PIN_AUD_DAD0_REG       ANA_REG_ADDR(0x0018)  
#define   ANA_PIN_AUD_DAD1_REG       ANA_REG_ADDR(0x001C)  
#define   ANA_PIN_AUD_ADSYNC_REG     ANA_REG_ADDR(0x0020)  
#define   ANA_PIN_AUD_ADD0_REG       ANA_REG_ADDR(0x0024)  
#define   ANA_PIN_ADI_D_REG          ANA_REG_ADDR(0x0028)  
#define   ANA_PIN_ADI_SYNC_REG       ANA_REG_ADDR(0x002C)  
#define   ANA_PIN_ADI_SCLK_REG       ANA_REG_ADDR(0x0030)  
#define   ANA_PIN_CLK_32K_REG        ANA_REG_ADDR(0x0034)  
#define   ANA_PIN_RFSDA_REG          ANA_REG_ADDR(0x0038)  
#define   ANA_PIN_RFSCK_REG          ANA_REG_ADDR(0x003C)  
#define   ANA_PIN_RFSEN_REG          ANA_REG_ADDR(0x0040)  
#define   ANA_PIN_FM_RXID0_REG       ANA_REG_ADDR(0x0044)  
#define   ANA_PIN_FM_RXID1_REG       ANA_REG_ADDR(0x0048)  
#define   ANA_PIN_FM_RXQD0_REG       ANA_REG_ADDR(0x004C)  
#define   ANA_PIN_FM_RXQD1_REG       ANA_REG_ADDR(0x0050)  
#define   ANA_PIN_ANA_IRQ_REG        ANA_REG_ADDR(0x0054)  
#define   ANA_PIN_ANA_FIQ_REG        ANA_REG_ADDR(0x0058)  
#define   ANA_PIN_EXT_RST_B_REG      ANA_REG_ADDR(0x005C)  
#define   ANA_PIN_CHIP_SLEEP_REG     ANA_REG_ADDR(0x0060)
#define   ANA_PIN_PTEST_REG          ANA_REG_ADDR(0x0064)  
#define   ANA_PIN_XTL_BUF_EN_REG     ANA_REG_ADDR(0x0068)  
#define   ANA_PIN_CLK_ITF_REG        ANA_REG_ADDR(0x006C)  
#define   ANA_PIN_TP_XL_REG          ANA_REG_ADDR(0x0070)  
#define   ANA_PIN_TP_XR_REG          ANA_REG_ADDR(0x0074)  
#define   ANA_PIN_TP_YU_REG          ANA_REG_ADDR(0x0078)  
#define   ANA_PIN_TP_YD_REG          ANA_REG_ADDR(0x007C)  
#define   ANA_PIN_PRODT_REG          ANA_REG_ADDR(0x0080)


/* Pinmap ctrl register Bit field value
 Reserved[31:16]    | slp ctrl[15]  | Reserved[14:13]   | Drv str sel[11:10]| func PU[9]    | func PD[8]    |
 func sel[7:5]      | slp out[4]    | PU[3]             | PD[2]             | input En[1]   | output En[0]  |
*/

#define PIN_Z_EN                0x00            // High-Z in sleep mode
#define PIN_O_EN                BIT_0           // Output enable in sleep mode
#define PIN_I_EN                BIT_1           // Input enable in sleep mode

#define PIN_SLP_Z               0x00            // High-Z in sleep mode,        instead of PIN_Z_EN
#define PIN_SLP_O               BIT_0           // Output enable in sleep mode, instead of PIN_O_EN
#define PIN_SLP_I               BIT_1           // Input enable in sleep mode,  instead of PIN_I_EN


#define PIN_SLP_OE_EN           BIT_0           //Output enable for chip deep sleep mode
#define PIN_SLP_IE_EN           BIT_1           //Input enable for chip deep sleep mode

#define PIN_SPD_EN              BIT_2           //Weak pull down for chip deep sleep mode
#define PIN_SPU_EN              BIT_3           //Weak pull up for chip deep sleep mode
#define PIN_SPX_EN              0x00            // Don't pull down or up
#define PIN_SPO_EN              BIT_4           //Setting output value for chip deep sleep mode
#define PIN_SPO_H               BIT_4           //Setting output HIGH value for chip deep sleep mode
#define PIN_SPO_L               0x00            //Setting output LOW value for chip deep sleep mode
#define PIN_SPO_X               0x00            //NO output value for chip deep sleep mode


#define PIN_FUNC_DEF            (0x00<<5)       //Function select,BIT5-7
#define PIN_FUNC_0              (0x00<<5)       // the same as PIN_FUNC_DEF , for code alignment
#define PIN_FUNC_1              (0x01<<5)
#define PIN_FUNC_2              (0x02<<5)
#define PIN_FUNC_3              (0x03<<5)
#define PIN_FUNC_4              (0x04<<5)       
#define PIN_FUNC_5              (0x05<<5)
#define PIN_FUNC_6              (0x06<<5)
#define PIN_FUNC_7              (0x07<<5)


#define PIN_FPD_EN              BIT_8           //Weak pull down for function mode
#define PIN_FPU_EN              BIT_9           //Weak pull up for function mode
#define PIN_FPX_EN              0x00            // Don't pull down or up

#define PIN_DS_0                (0x00<<10)           // Driver strength level 0 
#define PIN_DS_1                (0x01<<10)           // Driver strength level 1 
#define PIN_DS_2                (0x02<<10)           // Driver strength level 2 
#define PIN_DS_3                (0x03<<10)           // Driver strength level 3 
#define PIN_SPC_EN              BIT_15          //Slp_sw_ctrl(for slp_out/ slp_ie/ slp_oe/ slp_wpd/ slp_wpu): 1: enable 0: disalbe
#define PIN_FPC_EN              0x00            //disable Slp_sw_ctrl, pin ie/oe controlled by selected Pin Function

/* Pinmap control register bit field value structure*/
typedef union _pinmap_ctl_reg_u
{
    volatile int dwValue;

    struct pinmap_ctl_reg_tag
    {
        volatile int SIM0_DA_wpus           :1;
        volatile int SIM1_DA_wpus           :1;
        volatile int SIM2_DA_wpus           :1;
        volatile int SCL0_wpus              :1; 
        volatile int SDA0_wpus              :1;
        volatile int KEYIN0_wpus            :1;
        volatile int KEYIN1_wpus            :1;
        volatile int KEYIN2_wpus            :1;
        volatile int KEYIN3_wpus            :1;
        volatile int KEYIN4_wpus            :1;
        volatile int KEYIN5_wpus            :1;                                    
        volatile int KEYIN6_wpus            :1;
        volatile int KEYIN7_wpus            :1;
        volatile int MTRSTN_wpus            :1;
        volatile int MTD0_wpus              :1;
        volatile int LCMD7_wpus             :1;
        volatile int LCMD8_wpus             :1;
        volatile int SIMCLK2_wpus           :1;
        volatile int U0CTS_wpus             :1;
        volatile int U0RTS_wpus             :1;
        volatile int U0RXD_wpus             :1;
        volatile int U0TXD_wpus             :1;
        volatile int wpd_ddsdpd             :1;
        volatile int wpd_lcmpd              :1;
        volatile int wpd_mempd              :1;
        volatile int wpd_memspd             :1;
        volatile int wpd_sim0pd             :1;
        volatile int wpd_sim1pd             :1;                                                                                                                                    
        volatile int wpd_sim2pd             :1;
        volatile int wpd_dd18pd             :1;
        volatile int wpd_campd              :1;
        volatile int wpd_ddiopd             :1;                                    
    } mBits;

} PINMAP_CTL_REG_U;



/* Pinmap ctrl register Bit field value
--------------------------------------------------------------------------------------------------------------------------
|                 |                 |            |            |              |       |       |            |              |
| Reserved[31:10] | Drv str sel[9:8]| func PU[7] | func PD[6] | func sel[5:4]| PU[3] | PD[2] | input En[1]| output En[0] |
|                 |                 |            |            |              |       |       |            |              |
--------------------------------------------------------------------------------------------------------------------------
*/

#define ANA_PIN_Z_EN                0x00            // High-Z in sleep mode
#define ANA_PIN_O_EN                BIT_0           // Output enable in sleep mode
#define ANA_PIN_I_EN                BIT_1           // Input enable in sleep mode

#define ANA_PIN_SPD_EN               BIT_2           // Pull down enable
#define ANA_PIN_SPU_EN               BIT_3           // Pull up enable
#define ANA_PIN_SPX_EN               0x00            // Don't pull down or up

#define ANA_PIN_FUNC_DEF            (0x00<<4)       //Function select,BIT4-5
#define ANA_PIN_FUNC_1              (0x01<<4)
#define ANA_PIN_FUNC_2              (0x02<<4)
#define ANA_PIN_FUNC_3              (0x03<<4)

#define ANA_PIN_FPD_EN           BIT_6           // Weak pull down for function mode
#define ANA_PIN_FPU_EN           BIT_7           // Weak pull up for function mode
#define ANA_PIN_FPX_EN           0x00            // Don't pull down or up

#define ANA_PIN_DS_0                (0x00<<8)           // Driver strength level 0 BIT8-9
#define ANA_PIN_DS_1                (0x01<<8)           // Driver strength level 1 BIT8-9
#define ANA_PIN_DS_2                (0x02<<8)           // Driver strength level 2 BIT8-9
#define ANA_PIN_DS_3                (0x03<<8)           // Driver strength level 3 BIT8-9

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
