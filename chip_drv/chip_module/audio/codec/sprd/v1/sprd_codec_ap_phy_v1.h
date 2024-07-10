/******************************************************************************
 ** File Name:      sprd_codec_ap_phy_v1.h                                    *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    codec ap control                                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifndef __SPRD_CODEC_AP_PHY_V1_H___
#define __SPRD_CODEC_AP_PHY_V1_H___
#include "Chip_plf_export.h"


//audif_enb
#define AUDIF_DAC_EN            (BIT_0)
#define AUDIF_ADC_EN            (BIT_1)

#define AUDIO_LOOP_MAP_SEL      (BIT_2)
#define AUDIO_ANA_LOOP_EN       (BIT_3)

//audif_clr
#define AUDIF_DAC_PATH_CLR      (BIT_0)
#define AUDIF_ADC_PATH_CLR      (BIT_1)

//audif_sync_ctl
#define DAC_DATA_TX_ADDR_SHIFT  (0)
#define DAC_DATA_TX_ADDR_MASK   (0x3<<DAC_DATA_TX_ADDR_SHIFT)

#define ADC_DATA_RX_ADDR_SHIFT  (2)
#define ADC_DATA_RX_ADDR_MASK   (0x3<<DAC_DATA_TX_ADDR_SHIFT)

//audif_ocptmr_ctl
#define OCP_PRECIS_SHIFT        (0)
#define OCP_PRECIS_MASK         (0x7<<OCP_PRECIS_SHIFT)

#define OCP_PD_THD_SHIFT        (3)
#define OCP_PD_THD_MASK         (0x7<<OCP_PD_THD_SHIFT)

//audif_otptmr_ctl
#define OTP_PRECIS_SHIFT        (0)
#define OTP_PRECIS_MASK         (0x7<<OTP_PRECIS_SHIFT)

#define OTP_PD_THD_SHIFT        (3)
#define OTP_PD_THD_MASK         (0x7<<OTP_PD_THD_SHIFT)

//audif_shutdown_ctl
#define HP_SHUTDOWN_EN          (BIT_0)
#define EAR_SHUTDOWN_EN         (BIT_1)
#define LO_SHUTDOWN_EN          (BIT_2)
#define LOR_SHUTDOWN_EN         (BIT_3)
#define PA_SHUTDOWN_EN0         (BIT_4)
#define PA_SHUTDOWN_EN1         (BIT_5)
#define PA_SHUTDOWN_EN2         (BIT_6)
#define AUDIO_CLK_SEL           (BIT_7)

//audif_int_en
#define HP_OCP_IRQ_EN           (BIT_0)
#define EAR_OCP_IRQ_EN          (BIT_1)
#define LOL_OCP_IRQ_EN          (BIT_2)
#define LOR_OCP_IRQ_EN          (BIT_3)
#define PA_OCP_IRQ_EN           (BIT_4)
#define OTP_IRQ_EN              (BIT_5)
#define OVP_IRQ_EN              (BIT_6)
#define AUDIO_POP_IRQ_EN        (BIT_7)

//audif_int_clr
#define HP_OCP_IRQ_CLR          (BIT_0)
#define EAR_OCP_IRQ_CLR         (BIT_1)
#define LOL_OCP_IRQ_CLR         (BIT_2)
#define LOR_OCP_IRQ_CLR         (BIT_3)
#define PA_OCP_IRQ_CLR          (BIT_4)
#define OTP_IRQ_CLR             (BIT_5)
#define OVP_IRQ_CLR             (BIT_6)
#define AUDIO_POP_IRQ_CLR       (BIT_7)

//audif_int_raw
#define HP_OCP_IRQ              (BIT_0)
#define EAR_OCP_IRQ             (BIT_1)
#define LOL_OCP_IRQ             (BIT_2)
#define LOR_OCP_IRQ             (BIT_3)
#define PA_OCP_IRQ              (BIT_4)
#define OTP_IRQ                 (BIT_5)
#define OVP_IRQ                 (BIT_6)
#define AUDIO_POP_IRQ           (BIT_7)

//audif_int_mask
#define HP_OCP_IRQ_MASK         (BIT_0)
#define EAR_OCP_IRQ_MASK        (BIT_1)
#define LOL_OCP_IRQ_MASK        (BIT_2)
#define LOR_OCP_IRQ_MASK        (BIT_3)
#define PA_OCP_IRQ_MASK         (BIT_4)
#define OTP_IRQ_MASK            (BIT_5)
#define OVP_IRQ_MASK            (BIT_6)
#define AUDIO_POP_IRQ_MASK      (BIT_7)

//audif_ovptmr_ctl
#define OVP_PRECIS_SHIFT        (0)
#define OVP_PRECIS_MASK         (0x7<<OVP_PRECIS_SHIFT)

#define OVP_PD_THD_SHIFT        (3)
#define OVP_PD_THD_MASK         (0x7<<OVP_PD_THD_SHIFT)


//pmur1
#define AUXMICBIAS_EN           (BIT_0)
#define MICBIAS_EN              (BIT_1)
#define VBO_EN                  (BIT_2)
#define VCM_BUF_EN              (BIT_3)
#define AUD_VCM_EN              (BIT_4)
#define BG_IBIAS_EN             (BIT_5)
#define AUD_BG_EN               (BIT_6)
#define VB_EN                   (BIT_7)

//pmur2

#define OVP_LDO_EN              (BIT_0)
#define OVP_PD                  (BIT_1)
#define PAR_EN                  (BIT_2)
#define PAR_LDO_EN              (BIT_3)
#define PAR_SW_EN               (BIT_4)
#define PA_EN                   (BIT_5)
#define PA_LDO_EN               (BIT_6)
#define PA_SW_EN                (BIT_7)

//pmur3
#define AUXMICBIAS_V_SHIFT      (1)
#define AUXMICBIAS_V_MASK       (0x3<<MICBIAS_V_SHIFT)

#define MICBIAS_V_SHIFT         (3)
#define MICBIAS_V_MASK          (0x3<<MICBIAS_V_SHIFT)

#define AUD_VCM_V_SHIFT         (5)
#define AUD_VCM_V_MASK          (0x7<<AUD_VCM_V_SHIFT)

//pmur4
#define AUD_BG_I_SHIFT          (0)
#define AUD_BG_I_MASK           (0x3<<AUD_BG_I_SHIFT)

#define AUD_BG_RST              (BIT_2)

#define PA_LDO_V_SHIFT          (3)
#define PA_LDO_V_MASK           (0x7<<PA_LDO_V_SHIFT)

#define PA_LDOOCP_PD            (BIT_6)
#define PA_SWOCP_PD             (BIT_7)

//pmur5
#define AUD_OTP_T_SHIFT         (3)
//#define AUD_OTP_T_MASK          (0xF<<AUD_OTP_T_SHIFT)
#define AUD_OTP_T_MASK          (0x7<<AUD_OTP_T_SHIFT)

#define AUD_OTP_PD              (BIT_7)


//pmur6
#define AUD_OVP_T_SHIFT         (1)
#define AUD_OVP_T_MASK          (0x7<<AUD_OVP_T_SHIFT)

#define OVP_DEG_EN              (BIT_4)

#define AUD_OVP_V_SHIFT         (5)
#define AUD_OVP_V_MASK          (0x7<<AUD_OVP_V_SHIFT)

//hibdr
#define HEAD_T_SHIFT            (2)
#define HEAD_T_MASK             (0x7<<HEAD_T_SHIFT)

#define HEAD_SEARCH_RST         (BIT_5)
#define HEAD_SEARCH_EN          (BIT_6)
#define HEADDETECT_PD           (BIT_7)

//aacr1
#define FM_REC_EN               (BIT_1)
#define AUD_ADCR_RST            (BIT_2)
#define AUD_ADCR_PD             (BIT_3)
#define AUD_ADCL_RST            (BIT_4)
#define AUD_ADCL_PD             (BIT_5)
#define AUD_ADCL_VREF_X15       (BIT_6)
#define AUD_ADC_IBUF_PD         (BIT_7)

//aacr2
#define ADC_PGAR_BUFN_EN        (BIT_0)
#define ADC_PGAR_BUFP_EN        (BIT_1)
#define ADC_PGAL_BUFN_EN        (BIT_2)
#define ADC_PGAL_BUFP_EN        (BIT_3)
#define ADC_PGAR_BYPASS         (BIT_4)
#define ADC_PGAR_PD             (BIT_5)
#define ADC_PGAL_BYPASS         (BIT_6)
#define ADC_PGAL_PD             (BIT_7)

//aaicr1
#define ADC_HEADMICN_ADCR       (BIT_0)
#define ADC_HEADMICP_ADCR       (BIT_1)
#define ADC_HEADMICN_ADCL       (BIT_2)
#define ADC_HEADMICP_ADCL       (BIT_3)
#define ADC_MICN_ADCR           (BIT_4)
#define ADC_MICP_ADCR           (BIT_5)
#define ADC_MICN_ADCL           (BIT_6)
#define ADC_MICP_ADCL           (BIT_7)

//aaicr2
#define ADC_AUXMICN_ADCR        (BIT_4)
#define ADC_AUXMICP_ADCR        (BIT_5)
#define ADC_AUXMICN_ADCL        (BIT_6)
#define ADC_AUXMICP_ADCL        (BIT_7)

//aaicr3
#define ADC_VCM_ADCR            (BIT_2)
#define ADC_AIR_ADCR            (BIT_3)
#define ADC_AIL_ADCR            (BIT_4)
#define ADC_VCM_ADCL            (BIT_5)
#define ADC_AIR_ADCL            (BIT_6)
#define ADC_AIL_ADCL            (BIT_7)

//acgr
#define ADC_PGAR_G_SHIFT        (0)
#define ADC_PGAR_G_MASK         (0xF<<ADC_PGAR_G_SHIFT)

#define ADC_PGAL_G_SHIFT        (4)
#define ADC_PGAL_G_MASK         (0xF<<ADC_PGAL_G_SHIFT)


//dacr
#define AUD_DACBUF_I_650        ~(BIT_5)
#define AUD_DACBUF_I_450        (BIT_5)
#define AUD_DACR_EN             (BIT_6)
#define AUD_DACL_EN             (BIT_7)

//daocr1
#define DAC_DACR_HPR            (BIT_0)
#define DAC_DACR_HPL            (BIT_1)
#define DAC_DACL_HPR            (BIT_2)
#define DAC_DACL_HPL            (BIT_3)
#define DAC_ADCPGAR_BUFP_HPR    (BIT_4)
#define DAC_ADCPGAR_BUFP_HPL    (BIT_5)
#define DAC_ADCPGAL_BUFN_HPR    (BIT_6)
#define DAC_ADCPGAL_BUFP_HPL    (BIT_7)

//daocr2
#define DAC_DACR_SPN            (BIT_0)
#define DAC_DACR_SPP            (BIT_1)
#define DAC_DACL_SPN            (BIT_2)
#define DAC_DACL_SPP            (BIT_3)
#define DAC_ADCPGAR_BUFN_SPN    (BIT_4)
#define DAC_ADCPGAR_BUFP_SPP    (BIT_5)
#define DAC_ADCPGAL_BUFN_SPN    (BIT_6)
#define DAC_ADCPGAL_BUFP_SPP    (BIT_7)

//daocr3
#define DAC_DACR_AORN           (BIT_0)
#define DAC_DACR_AORP           (BIT_1)
#define DAC_DACL_AORN           (BIT_2)
#define DAC_DACL_AORP           (BIT_3)
#define DAC_ADCPGAR_BUFN_AORN   (BIT_4)
#define DAC_ADCPGAR_BUFP_AORP   (BIT_5)
#define DAC_ADCPGAL_BUFN_AORN   (BIT_6)
#define DAC_ADCPGAL_BUFP_AORP   (BIT_7)

//daocr4
#define DAC_DACL_EARN           (BIT_6)
#define DAC_DACL_EARP           (BIT_7)

//dcr1
#define AUD_DIFF_EN             (BIT_2)
#define AUD_AOLR_EN             (BIT_3)
#define AUD_AOL_EN              (BIT_4)
#define AUD_EAR_EN              (BIT_5)
#define AUD_HPR_EN              (BIT_6)
#define AUD_HPL_EN              (BIT_7)

//dcr2
#define DRV_STOP_EN             (BIT_1)
#define PA_SS_RST               (BIT_2)
#define PA_SS_EN                (BIT_3)
#define PA_DEMI_EN              (BIT_4)

#define PA_DTRI_F_SHIFT         (5)
#define PA_DTRI_F_MASK          (0x3<<PA_DTRI_F_SHIFT)

#define PA_D_EN                 (BIT_7)


//dcr3
#define AUD_DRV_OCP_HP_I_130    (BIT_0)
#define AUD_DRV_OCP_HP_I_190    ~(BIT_0)
#define AUD_DRV_OCP_EAR_I_300   (BIT_1)
#define AUD_DRV_OCP_EAR_I_450   ~(BIT_1)
#define AUD_DRV_OCP_LO_I_4      (BIT_2)
#define AUD_DRV_OCP_LO_I_6      ~(BIT_2)
#define AUD_DRV_OCP_PA_I_500    (BIT_3)
#define AUD_DRV_OCP_PA_I_700    ~(BIT_3)

#define AUD_DRV_OCP_HP_PD       (BIT_4)
#define AUD_DRV_OCP_EAR_PD      (BIT_5)
#define AUD_DRV_OCP_LO_PD       (BIT_6)
#define AUD_DRV_OCP_PA_PD       (BIT_7)

//dcr4

#define PA_SS_T_SHIFT           (2)
#define PA_SS_T_MASK            (0x7<<PA_SS_T_SHIFT)

#define PA_SS_32K_EN            (BIT_5)

#define PA_SS_F_SHIFT           (6)
#define PA_SS_F_MASK            (0x3<<PA_SS_F_SHIFT)

//dcgr1
#define DAC_HPR_G_SHIFT         (0)
#define DAC_HPR_G_MASK          (0xF<<DAC_HPR_G_SHIFT)

#define DAC_HPL_G_SHIFT         (4)
#define DAC_HPL_G_MASK          (0xF<<DAC_HPL_G_SHIFT)


//dcgr2
#define DAC_EAR_G_SHIFT         (4)
#define DAC_EAR_G_MASK          (0xF<<DAC_EAR_G_SHIFT)


//dcgr3
#define DAC_PAR_G_SHIFT         (0)
#define DAC_PAR_G_MASK          (0xF<<DAC_PAR_G_SHIFT)

#define DAC_PA_G_SHIFT          (4)
#define DAC_PA_G_MASK           (0xF<<DAC_PA_G_SHIFT)


//pnrcr1
#define HP_POP_SOFTCHAG_EN      (BIT_1)
#define HP_POP_CHAG_PD          (BIT_2)

#define HP_POP_STEP_SHIFT       (3)
#define HP_POP_STEP_MASK        (0x7<<HP_POP_STEP_SHIFT)

#define HP_POP_CTL_SHIFT        (6)
#define HP_POP_CTL_MASK         (0x3<<HP_POP_CTL_SHIFT)

//pnrcr2

#define HP_POP_SPEED_SHIFT      (0)
#define HP_POP_SPEED_MASK       (0xFF<<HP_POP_SPEED_SHIFT)

//pnrcr3
#define HP_POP_COMP_EN          (BIT_4)

#define HP_POP_CHG_V_SHIFT      (5)
#define HP_POP_CHG_V_MASK       (0x3<<HP_POP_CHG_V_SHIFT)

#define HP_POP_CHGV_EN          (BIT_7)

//ccr
#define AUD_DRV_CLK_EN          (BIT_3)
#define AUD_DAC_CLK_EN          (BIT_4)
#define AUD_ADC_CLK_RST         (BIT_5)
#define AUD_ADC_CLK_PD          (BIT_6)
#define AUD_CLK_REVERSE         (BIT_7)

//ifr1
#define AUD_OVP_FLAG            (BIT_2)
#define HP_POP_COMP_FLAG        (BIT_3)
#define HP_POP_FLG_SHIFT        (4)
#define HP_POP_FLG_MASK         (0x3<<HP_POP_FLG_SHIFT)
#define AUD_HEAD_INSERT         (BIT_6)
#define AUD_HEAD_BUTTON         (BIT_7)

//ifr2
#define AUD_DRV_OCP_PA_SHIFT    (0)
#define AUD_DRV_OCP_PA_MASK     (0x3<<AUD_DRV_OCP_PA_SHIFT)

#define AUD_DRV_OCP_PAR_SHIFT   (2)
#define AUD_DRV_OCP_PAR_MASK    (0x3<<AUD_DRV_OCP_PAR_SHIFT)

#define AUD_DRV_OTP_FLAG        (BIT_4)

//ifr3
#define AUD_DRV_OCP_HP_SHIFT    (0)
#define AUD_DRV_OCP_HP_MASK     (0x3<<AUD_DRV_OCP_HP_SHIFT)

#define AUD_DRV_OCP_EAR_SHIFT   (2)
#define AUD_DRV_OCP_EAR_MASK    (0x3<<AUD_DRV_OCP_EAR_SHIFT)

#define AUD_DRV_OCP_LO_SHIFT    (4)
#define AUD_DRV_OCP_LO_MASK     (0x3<<AUD_DRV_OCP_LO_SHIFT)

#define AUD_DRV_OCP_LOR_SHIFT   (6)
#define AUD_DRV_OCP_LOR_MASK    (0x3<<AUD_DRV_OCP_LOR_SHIFT)

typedef struct{
    uint32  audif_enb;          //0000
    uint32  audif_clr;          //0004
    uint32  audif_sync_ctl;     //0008
    uint32  audif_ocptmr_ctl;   //000C
    uint32  audif_otptmr_ctl;   //0010
    uint32  audif_shutdown_ctl; //0014
    uint32  audif_int_en;       //0018
    uint32  audif_int_clr;      //001C
    uint32  audif_int_raw;      //0020
    uint32  audif_int_mask;     //0024
    uint32  audif_ovptmr_ctl;   //0028
    uint32  reserved0;          //002C
    uint32  reserved1;          //0030
    uint32  reserved2;          //0034
    uint32  reserved3;          //0038
    uint32  reserved4;          //003C
    uint32  pmur1;          //0040
    uint32  pmur2;          //0044
    uint32  pmur3;          //0048
    uint32  pmur4;          //004C
    uint32  pmur5;          //0050
    uint32  pmur6;          //0054
    uint32  hibdr;          //0058
    uint32  aacr1;          //005C
    uint32  aacr2;          //0060
    uint32  aaicr1;         //0064
    uint32  aaicr2;         //0068
    uint32  aaicr3;         //006C
    uint32  acgr;           //0070
    uint32  dacr;           //0074
    uint32  daocr1;         //0078
    uint32  daocr2;         //007C
    uint32  daocr3;         //0080
    uint32  dcr1;           //0084
    uint32  dcr2;           //0088
    uint32  dcr3;           //008C
    uint32  dcr4;           //0090
    uint32  dcgr1;          //0094
    uint32  dcgr2;          //0098
    uint32  dcgr3;          //009C
    uint32  pnrcr1;         //00A0
    uint32  pnrcr2;         //00A4
    uint32  pnrcr3;         //00A8
    uint32  ccr;            //00AC
    uint32  ifr1;           //00B0
    uint32  ifr2;           //00B4
    uint32  ifr3;           //00B8
    uint32  daocr4;         //00BC
}SPRD_CODEC_AP_REG_CTL_T;


#endif //__SPRD_CODEC_AP_PHY_V1_H___
