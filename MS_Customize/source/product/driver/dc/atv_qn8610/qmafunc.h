/************************************************Copyright(c)***************************************************
**                                   Quintic(Beijing) Microelectronics Co,Ltd.
**                                   
**                                   http:\\www.quinticcorp.com 
**
**--------------------File Info----------------------------------------------------------------
** File Name:       qmafunc.h
** Last Modified Date: 
** Last Version:     
** Descriptions: The qmafunc.h is used to driver the detailed function 
**
**----------------------------------------------------------------------------------------
** Created by:   ws
** Created Date: 2009-08-26
** Version:      0.1
** Descriptions: 
**
**-------------------------------------------------------------------------------------
** Modified by:     
** Modified Date :  
** Version:           
** Descriptions:     For QN8610 B1      
**
**************************************************************************************************/
#ifndef _QMAFUNC_H_
#define _QMAFUNC_H_

#define QNR_SYSTEM1                      0x00
#define QNR_SYSTEM1_SWRST                0x0080
#define QNR_SWRST_KEEP                   0x00
#define QNR_SWRST_RESET                  0x80
#define QNR_SYSTEM1_RECAL                0x0040
#define QNR_RECAL_NOTACT                 0x00
#define QNR_RECAL_FORCE_FSM              0x40
#define QNR_SYSTEM1_RX_REQ               0x0020
#define QNR_RX_REQ_ENTER                 0x20
#define QNR_RX_REQ_EXIT                  0x00
#define QNR_SYSTEM1_MPD                  0x0010
#define QNR_MPD_PU                       0x00
#define QNR_MPD_PD                       0x10
#define QNR_SYSTEM1_SEL_EXT_DVDD12       0x0008
#define QNR_SEL_EXT_DVDD12_INTERNAL      0x00
#define QNR_SEL_EXT_DVDD12_EXTERNAL      0x08
#define QNR_SYSTEM1_PD_DVREG12           0x0004
#define QNR_PD_DVREG12_PU                0x00
#define QNR_PD_DVREG12_PD                0x04
#define QNR_SYSTEM1_DVDD12_SW            0x0002
#define QNR_DVDD12_SW_EN                 0x02
#define QNR_DVDD12_SW_DIS                0x00
#define QNR_SYSTEM1_RX_MODE              0x0001
#define QNR_RX_MODE_ATV                  0x00
#define QNR_RX_MODE_FM                   0x01
                                         
#define QNR_SYSTEM2                      0x01
#define QNR_SYSTEM2_MUTE                 0x0101
#define QNR_MUTE_ENABLE                  0x01
#define QNR_MUTE_DISABLE                 0x00
#define QNR_SYSTEM2_AUD_TEST             0x0102
#define QNR_AUD_NORMAL                   0x00
#define QNR_AUD_TEST                     0X02
#define QNR_SYSTEM2_I2S_EN               0x0120
#define QNR_SYSTEM2_ANALOG_AUDIO         0x00
#define QNR_SYSTEM2_DIGITAL_AUDIO        0x20
#define QNR_SYSTEM2_IMR                  0x0180
#define QNR_SYSTEM2_IMR_LOW              0x00
#define QNR_SYSTEM2_IMR_UPPER            0x80
                                         
#define QNR_TV_STD1                      0x02
#define QNR_TV_STD1_VID_STD              0x0203
#define QNR_TV_STD1_VID_STD_NTSC         0x00
#define QNR_TV_STD1_VID_STD_PALM         0x01
#define QNR_TV_STD1_VID_STD_PALD         0x02
#define QNR_TV_STD1_VID_STD_SECAM        0x03
#define QNR_TV_STD1_CHN_BW               0x020c
#define QNR_CHN_BW_6MHz                  0x04
#define QNR_CHN_BW_7MHz                  0x08
#define QNR_CHN_BW_8MHz                  0x0c
#define QNR_TV_STD1_AUD_CAR              0x02c0
#define QNR_AUD_CAR_65MHz                0xc0
#define QNR_AUD_CAR_60MHz                0x80
#define QNR_AUD_CAR_55MHz                0x40
#define QNR_AUD_CAR_45MHz                0x00
#define QNR_TV_STD1_VID_SUBCAR           0x0230
#define QNR_VID_SUBCAR_3579              0x00
#define QNR_VID_SUBCAR_3575              0x10
#define QNR_VID_SUBCAR_3582              0x20
#define QNR_VID_SUBCAR_4433              0x30
#define QNR_FSC_11                       0x30
                                         
#define QNR_TV_STD2                      0x03
#define QNR_TV_STD2_AUD_STD              0x0303
#define QNR_AUD_STD_A2                   0x01
#define QNR_AUD_STD_BTSC                 0x00
#define QNR_TV_STD2_SAP_EN               0x0308
#define QNR_SAP_EN                       0x08
#define QNR_SAP_DIS                      0x00
#define QNR_TV_STD2_AUD_SEL              0x0330
#define QNR_TV_STD2_DEEMP_BP             0x0340
#define QNR_DEEMP_EN                     0x00
#define QNR_DEEMP_BP                     0x40
#define QNR_TV_STD2_DEEMP_TC             0x0380
#define QNR_DEEMP_50US                   0x00
#define QNR_DEEMP_75US                   0x80
                                         
#define QNR_CID1                         0x05
#define QNR_CID1_REG                     0x05FF
#define QNR_CID2                         0x06
#define QNR_CID2_REG                     0x06FF
#define QNR_STATUS1                      0x07
#define QNR_STATUS1_FM_CCA_FAIL          0x0780
#define QNR_FM_CCA_OK                    0x00
#define QNR_FM_CCA_FAIL                  0x80
#define QNR_STATUS2                      0x08
#define QNR_STATUS2_AUD_MODE             0x08c0
#define QNR_ATV_AUD_STEREO               0x00
#define QNR_ATV_AUD_MONO                 0x40
#define QNR_STATUS2_VOL_SAT              0x0820
#define QNR_VOL_SAT_NOT_SATURATED        0x00
#define QNR_VOL_SAT_SATURATED            0x20
#define QNR_STATUS2_ASYM_MODE_CHG         0x0810
#define QNR_ASYM_MODE_NOT_CHANGED         0x00
#define QNR_ASYM_MODE_CHANGED             0x10
#define QNR_STATUS2_FSC_LOCK             0x0808
#define QNR_FSC_LOCK_NOT                 0x00
#define QNR_FSC_LOCK_WEN                 0x08
#define QNR_STATUS2_SYNC_FSM             0x0806
#define QNR_SYNC_FSM_HSYNC_SEARCHING     0x00
#define QNR_SYNC_FSM_HSYNC_CONFIRMING    0x02
#define QNR_SYNC_FSM_VSYNC_SEARCHING     0x04
#define QNR_SYNC_FSM_TRACKING            0x06
#define QNR_STATUS2_VID_LOCK             0x0801
#define QNR_VID_LOCK_NOT                 0x00
#define QNR_VID_LOCK_WEN                 0x01
#define QNR_GPIO_CTRL                    0x09
#define QNR_GPIO1_SEL                    0x09c0
#define QNR_GPIO1_SEL_INT                0x00
#define QNR_GPIO1_SEL_FIELD              0x40
#define QNR_GPIO1_SEL_GPIO               0x80
#define QNR_GPIO0_SEL                    0x0930
#define QNR_GPIO0_SEL_SEB                0x00
#define QNR_GPIO0_SEL_CLKOUT             0x10
#define QNR_GPIO0_SEL_GPIO               0x20
#define QNR_SDA_DRV                      0x09c0
#define QNR_SDA_DRV_NORMAL               0x00
#define QNR_SDA_DRV_12                   0x04
#define QNR_SDA_DRV_14                   0x08
#define QNR_SDA_DRV_18                   0x0c
#define QNR_GPIO_DATA                    0x0a
#define QNR_GPIO_DATA_GPIO1_I            0x0a80
#define QNR_GPIO1_I_LOW                  0x00
#define QNR_GPIO1_I_HIGH                 0x80
#define QNR_GPIO_DATA_GPIO0_I            0x0a40
#define QNR_GPIO0_I_LOW                  0x00
#define QNR_GPIO0_I_HIGH                 0x40
#define QNR_GPIO_DATA_GPIO1_OUT          0x0a20
#define QNR_GPIO1_OUT_LOW                0x00
#define QNR_GPIO1_OUT_HIGH               0x20
#define QNR_GPIO_DATA_GPIO0_OUT          0x0a10
#define QNR_GPIO0_OUT_LOW                0x00
#define QNR_GPIO0_OUT_HIGH               0x10
#define QNR_GPIO_DATA_GPIO1_IE           0x0a08
#define QNR_GPIO1_IE_DIS                 0x00
#define QNR_GPIO1_IE_EN                  0x08
#define QNR_GPIO_DATA_GPIO0_IE           0x0a04
#define QNR_GPIO0_IE_DIS                 0x00
#define QNR_GPIO0_IE_EN                  0x04
#define QNR_GPIO_DATA_GPIO1_OE           0x0a02
#define QNR_GPIO1_OE_DIS                 0x00
#define QNR_GPIO1_OE_EN                  0x02
#define QNR_GPIO_DATA_GPIO0_OE           0x0a01
#define QNR_GPIO0_OE_DIS                 0x00
#define QNR_GPIO0_OE_EN                  0x01
#define QNR_CON                          0x0B
#define QNR_CON_REG                      0x0BFF
#define QNR_CON_VALUE                    0x26
#define QNR_BRIGHT                       0x0C
#define QNR_BRIGHT_REG                   0x0CFF
#define QNR_BRIGHT_VALUE                 0x16
#define QNR_LUMA_GAIN                    0x0D
#define QNR_LUMA_GAIN_REG                0x0DFF
#define QNR_LUMA_GAIN_VALUE              0x20
#define QNR_CHROMA_GAIN                  0x0E
#define QNR_CHROMA_GAIN_REG              0x0EFF
#define QNR_CHROMA_GAIN_VALUE            0x3c
#define QNR_HUE                          0x0F
#define QNR_HUE_REG                      0x0FFF

#define QNR_VID_CTRL1                    0x10
#define QNR_VID_CTRL1_VID_EN             0x1001
#define QNR_VID_VID_DIS                  0x00
#define QNR_VID_VID_EN                   0x01
#define QNR_VID_CTRL1_ITU_MODE           0x1002
#define QNR_VID_ITU_MODE_BT601           0x00
#define QNR_VID_ITU_MODE_BT656           0x02
#define QNR_VID_CTRL1_VID_SRC            0x1004
#define QNR_VID_SRC_ATV_DECODER          0x00
#define QNR_VID_SRC_TEST_PATTERN         0x04
#define QNR_VID_CTRL1_CLR_TYPE           0x1008
#define QNR_CLR_TYPE_YCbCr               0x00
#define QNR_CLR_TYPE_RGB                 0x08
#define QNR_VID_CTRL1_DECI_EN            0x1010
#define QNR_DECI_EN                      0x10
#define QNR_DECI_DIS                     0x00
#define QNR_VID_CTRL1_DOUT_GATE          0x1020
#define QNR_DOUT_RANDOM                  0x20
#define QNR_DOUT_ZERO                    0x00
#define QNR_VID_CTRL1_HUE98              0x10c0

#define QNR_VID_CTRL2                    0x11
#define QNR_VID_CTRL2_BIT_SWAP           0x1101
#define QNR_BIT_SWAP_MLSB                0x00
#define QNR_BIT_SWAP_LMSB                0x01
#define QNR_VID_CTRL2_ORDER              0x1106
#define QNR_ORDER_YCbYCr                 0x00
#define QNR_ORDER_YCrYCb                 0x02
#define QNR_ORDER_CbYCrY                 0x04
#define QNR_ORDER_CrYCbY                 0x06
#define QNR_VID_CTRL2_PCLKPOL            0x1108
#define QNR_PCLKPOL_NOT_INVERT           0x00
#define QNR_PCLKPOL_INVERT               0x08
#define QNR_VID_CTRL2_HS_POL             0x1110
#define QNR_HS_POL_NOT_INVERT            0x00    
#define QNR_HS_POL_INVERT                0x10
#define QNR_VID_CTRL2_VS_POL             0x1120
#define QNR_VS_POL_NOT_INVERT            0x00    
#define QNR_VS_POL_INVERT                0x20
#define QNR_VID_CTRL2_FS_SEL             0x1140
#define QNR_FS_SEL_30FPS                 0x00
#define QNR_FS_SEL_15FPS                 0x40
#define QNR_VID_CTRL2_YUVRANGE           0x1180
#define QNR_STD_SCALE                    0x00
#define QNR_FULL_SCALE                   0x80
#define QNR_AVID_CFG1                    0x12
#define QNR_AVID_CFG1_START_PIXEL        0x12FF
#define QNR_AVID_CFG2                    0x13
#define QNR_AVID_CFG2_START_LINE1        0x13FF
#define QNR_AVID_CFG3                    0x14
#define QNR_AVID_CFG3_START_LINE2        0x14FF
#define QNR_AVID_CFG4                    0x15
#define QNR_AVID_CFG4_START_LINE1        0x1503
#define QNR_AVID_CFG4_START_LINE2        0x150c
#define QNR_AVID_CFG4_PIXEL_NUM          0x1530
#define QNR_AVID_CFG4_VBI_NUM_SEL        0x1540
#define VBI_NUM_SEL_ONE                  0x40
#define VBI_NUM_SEL_ALL                  0x00
#define QNR_AVID_CFG4_LINE_NUM           0x1580
#define QNR_AVID_CFG5_PIXEL_NUM          0x16ff
#define QNR_AVID_CFG6_LINE_NUM           0x17ff
#define QNR_VS_CFG1_START1               0x18ff
#define QNR_VS_CFG2_STOP1                0x19ff
#define QNR_VS_CFG3_START2               0x1aff
#define QNR_VS_CFG4_STOP2                0x1bff
#define QNR_VS_CFG5                      0x1c
#define QNR_VS_CFG5_START1               0x1c03
#define QNR_VS_CFG5_STOP1                0x1c0c
#define QNR_VS_CFG5_START2               0x1c30
#define QNR_VS_CFG5_STOP2                0x1cc0
#define QNR_VID_IO_CTRL                  0x1d
#define QNR_PCLK_CTRL_PCLK_CTRL          0x1dc0
#define QNR_PCLK_CTRL_TRI_STATE          0x00
#define QNR_PCLK_CTRL_DRIVING_0          0x40
#define QNR_PCLK_CTRL_DRIVING_1          0x80
#define QNR_PCLK_CTRL_NORMAL             0xc0
#define QNR_PCLK_CTRL_DOUT_CTRL          0x1d30
#define QNR_DOUT_CTRL_TRI_STATE          0x00
#define QNR_DOUT_CTRL_DRIVING_0          0x10
#define QNR_DOUT_CTRL_DRIVING_1          0x20
#define QNR_DOUT_CTRL_NORMAL             0x30
#define QNR_PCLK_CTRL_PCLK_DRV           0x1d0c
#define QNR_PCLK_DRV_LOW                 0x00
#define QNR_PCLK_DRV_MIDDLE              0x04
#define QNR_PCLK_DRV_HIGH                0x08
#define QNR_PCLK_DRV_HIGEST              0x0c
#define QNR_PCLK_CTRL_DOUT_DRV           0x1d03
#define QNR_DOUT_DRV_LOW                 0x00
#define QNR_DOUT_DRV_MIDDLE              0x01
#define QNR_DOUT_DRV_HIGH                0x02
#define QNR_DOUT_DRV_HIGEST              0x03

#define QNR_SCALER_CTRL1                 0x20
#define QNR_SCALER_CTRL1_XDT             0x20FF

#define QNR_SCALER_CTRL2                 0x21
#define QNR_SCALER_CTRL2_YDT             0x217F

#define QNR_SCALER_CTRL3                 0x22
#define QNR_SCALER_CTRL3_HSCALE_RATIO    0x2230
#define QNR_SCALER_CTRL3_TF_BP           0x2240
#define QNR_TF_EN                        0x00
#define QNR_TF_BP                        0x40
#define QNR_SCALER_CTRL3_SCALER_EN       0x2280
#define QNR_SCALER_EN                    0x80
#define QNR_SCALER_DIS                   0x00
#define QNR_SCALER_CTRL3_FIELD_SEL       0x220c
#define QNR_FIELD_SEL_ODD                0x00
#define QNR_FIELD_SEL_EVEN               0x04
#define QNR_FIELD_SEL_BOTH               0x08

#define QNR_SCALER_CTRL4                 0x23
#define QNR_SCALER_CTRL4_VSCALE_EN       0x2301
#define QNR_VSCALE_EN_PS                 0x01
#define QNR_VSCALE_EN_NPM                0x00
#define QNR_SCALER_CTRL4_VSCALE_WEN      0x2302
#define QNR_VSCALE_WEN_FIXED             0x20
#define QNR_VSCALE_WEN_NO_FIXED          0x00

#define QNR_VID_ASYM                      0x2c
#define QNR_VID_ASYM_WEN                  0x2c80
#define QNR_ASYM_WEN_INTERNAL             0x00
#define QNR_ASYM_WEN_REG                  0x80

#define QNR_OUT_LINE_DLY                 0x2e
#define QNR_OUT_LINE_DLY_REG             0x2eFF

#define QNR_AUD_CTRL1                    0x2f
#define QNR_AUD_CTRL1_GAIN_DIG           0x2f3f
#define QNR_GAIN_DIG00                   0x00
#define QNR_GAIN_DIG01                   0x01
#define QNR_GAIN_DIG23                   0x17
#define QNR_GAIN_DIG41                   0x29
#define QNR_AUD_CTRL1_AEQ_BP             0x2f80
#define QNR_AEQ_BP_EN                    0x00
#define QNR_AEQ_BP_BP                    0x80
                                         
#define QNR_AUD_CTRL2                    0x30
#define QNR_AUD_CTRL2_Q_BS               0x3006
#define QNR_Q_BS_100                     0x00
#define QNR_Q_BS_125                     0x04
#define QNR_Q_BS_150                     0x08
#define QNR_Q_BS_200                     0x0c
#define QNR_AUD_CTRL2_A2_CH2_OPT         0x3020
#define QNR_A2_CH2_OPT_ADD242            0x00
#define QNR_A2_CH2_OPT_SUB242            0x20
#define QNR_AUD_CTRL2_CH_WEN             0x3040
#define QNR_CH_WEN_DIS                   0x40
#define QNR_CH_WEN_EN                    0x00
#define QNR_AUD_CTRL2_CCA                0x3080
#define QNR_CCA_EN                       0x80
#define QNR_CCA_DIS                      0x00
#define QNR_AUD_CTRL2_I2S_SCKINV         0x3001
#define QNR_I2S_SCKINV_NOT_INV           0x00
#define QNR_I2S_SCKINV_INV               0x01

#define QNR_AUD_CTRL3                    0x31
#define QNR_AUD_CTRL3_FC_BS              0x3160
#define QNR_FC_BS_60HZ                   0x00
#define QNR_FC_BS_70HZ                   0x20
#define QNR_FC_BS_80HZ                   0x40
#define QNR_FC_BS_100HZ                  0x60
#define QNR_AUD_CTRL3_GAIN_BS            0x311f


#define QNR_I2S_CTRL                     0x32
#define QNR_I2SBW                        0x32c0
#define QNR_I2SBW_8BIT                   0x00
#define QNR_I2SBW_16BIT                  0x40
#define QNR_I2SBW_24BIT                  0x80
#define QNR_I2SBW_32BIT                  0xc0
#define QNR_I2SDRATE                     0x3230
#define QNR_I2SDRATE_32K                 0x00
#define QNR_I2SDRATE_40K                 0x10
#define QNR_I2SDRATE_441K                0x20
#define QNR_I2SDRATE_48K                 0x30
#define QNR_I2SMASTER                    0x3208
#define QNR_I2SMASTER_M                  0x08
#define QNR_I2SMASTER_S                  0x00
#define QNR_I2SMODE                      0x3207
#define QNR_I2SMODE_MSB                  0x00
#define QNR_I2SMODE_I2S                  0x01
#define QNR_I2SMODE_DSP1                 0x02
#define QNR_I2SMODE_DSP2                 0x03
#define QNR_I2SMODE_LSB                  0x04
#define QNR_FM_CH_START                  0x33
#define QNR_FM_CH_START_REG              0x33ff
#define QNR_FM_CH_STOP                   0x34
#define QNR_FM_CH_STOP_REG               0x34ff
#define QNR_CH_STEP                      0x35
#define QNR_FM_CH_STEP_CH98              0x3503
#define QNR_FM_CH_STEP_START98           0x350c
#define QNR_FM_CH_STEP_STOP98            0x3530
#define QNR_FM_CH_STEP_REG               0x35c0
#define QNR_FM_CH_STEP_50KHZ             0x00
#define QNR_FM_CH_STEP_100KHZ            0x40
#define QNR_FM_CH_STEP_200KHZ            0x80
#define QNR_FM_CH                        0x36
#define QNR_FM_CH_REG                    0x36FF

#define QNR_TUNER_CTRL                   0x40
#define QNR_TUNER_CTRL_DAC_GAIN          0x4007
#define QNR_DAC_GAIN0                    0x07
#define QNR_DAC_GAIN1                    0x06
#define QNR_DAC_GAIN2                    0x05
#define QNR_DAC_GAIN3                    0x04
#define QNR_DAC_GAIN4                    0x03
#define QNR_DAC_GAIN5                    0x02
#define QNR_DAC_GAIN6                    0x01
#define QNR_DAC_GAIN7                    0x00
#define QNR_TUNER_CTRL_BAND_SEL          0x40c0
#define QNR_BAND_SEL_UHF                 0xc0
#define QNR_BAND_SEL_VHFIII              0x80
#define QNR_BAND_SEL_VHFI_FM             0x40
#define QNR_TUNER_CTRL_DIV23_SEL         0x4020
#define QNR_DIV23_SEL_2                  0x20
#define QNR_DIV23_SEL_3                  0x00
#define QNR_TUNER_CTRL_DIV24_SEL         0x4010
#define QNR_DIV24_SEL_2                  0x10
#define QNR_DIV24_SEL_4                  0x00

#define QNR_XTAL_1                       0x41
#define QNR_XTAL_1_DIV                   0x4140
#define QNR_XTAL_1_DIV_1                 0x00    
#define QNR_XTAL_1_DIV_2                 0x40
#define QNR_REF_DIV_1                    0x43
#define QNR_REF_DIV_1_REG                0x43FF
#define QNR_REF_DIV_2                    0x44
#define QNR_REF_DIV_2_REG                0x44fF
#define QNR_REF_DIV_2_XTAL_DIV_MSB       0x4407
#define QNR_REF_DIV_2_CLK_DLT_SB         0x44f8
#define QNR_REF_DIV_3                    0x45
#define QNR_REF_DIV_3_REG                0x45FF
#define QNR_CHANNEL_CONFIG1              0x46
#define QNR_CHANNEL_CONFIG1_LO_VCO_SEL   0x4640
#define QNR_CHANNEL_CONFIG1_REG          0x463F
#define QNR_CHANNEL_CONFIG1_CLK2_EN       0x4680
#define QNR_CLK2_EN                       0x80
#define QNR_CLK2_DIS                      0x00
#define QNR_CHANNEL_CONFIG2              0x47
#define QNR_CHANNEL_CONFIG2_REG          0x470F
#define QNR_CHANNEL_CONFIG3              0x48
#define QNR_CHANNEL_CONFIG3_REG          0x48FF
#define QNR_CHANNEL_CONFIG4              0x49

#define QNR_CHANNEL_CONFIG4_REG          0x49FF
       
//setting audio parameters type
#define QNM_CONFIG_MUTE                  0
#define QNM_CONFIG_TV_AUD_STD            1
#define QNM_CONFIG_TV_AUD_OUTPUT         2
#define QNM_CONFIG_VOLUME                3
       
//setting TV audio standard
#define QNM_TV_AUD_STD_BTSC              0
#define QNM_TV_AUD_STD_A2                1

//setting TV audio output selection
#define QNM_TV_AUD_STEREO                0
#define QNM_TV_AUD_FAP                   1
#define QNM_TV_AUD_SAP                   2
#define QNM_TV_AUD_FSAP                  3

//setting video parameters type for adjustment
#define QNM_CONFIG_BRIGHTNESS            0
#define QNM_CONFIG_CONTRAST              1
#define QNM_CONFIG_HUE                   2
#define QNM_CONFIG_COLOR_SATURATION      3
#define QNM_CONFIG_LUMA                  4
#define QNM_CONFIG_BLACK_WHITE           5
#define QNM_CONFIG_TFLT                  6
#define QNM_CONFIG_HOLD                  7

// color kill setting
#define QNM_CKILL_NONE                   0
#define QNM_CKILL_SETTING_1              1
#define QNM_CKILL_SETTING_2              2
#define QNM_CKILL_SETTING_3              3
#define QNM_CKILL_SETTING_4              4
#define QNM_CKILL_SETTING_5              5
#define QNM_CKILL_SETTING_6              6

// resolution setting
#define QNM_CFG_START_PIXEL              0x05
#define QNM_CFG_START_LINE1_PS           0x0a       
#define QNM_CFG_START_LINE1_NPM          0x05
#define QNM_CFG_START_LINE2_PS           0x143       
#define QNM_CFG_START_LINE2_NPM          0x10C       
#define QNM_CFG_OUT_LINE_DLY_640_PS      0x22       
#define QNM_CFG_VS_START1                1
#define QNM_CFG_VS_STOP1                 3// 1 // 3
#define QNM_CFG_VS_START2_PS           316//  1//316       
#define QNM_CFG_VS_STOP2_PS              316
#define QNM_CFG_VS_START2_NPM           265 // 1//265       
#define QNM_CFG_VS_STOP2_NPM             265

// resolution setting level
enum
{
	QNM_RESOLUTION_320_240 = 0,
	QNM_RESOLUTION_360_240,           
	QNM_RESOLUTION_400_240,          
	QNM_RESOLUTIOM_640_240,           
	QNM_RESOLUTIOM_176_220,          
	QNM_RESOLUTIOM_160_128,          
	QNM_RESOLUTIOM_176_144,          
	QNM_RESOLUTIOM_220_176,           
	QNM_RESOLUTIOM_352_288,           
	QNM_RESOLUTIOM_480_300,           
	QNM_RESOLUTIOM_640_300,  
	QNM_RESOLUTIOM_COUNT,            
};

// TF parameters level
typedef enum
{
	QNM_TF_LEVEL_0=0,
	QNM_TF_LEVEL_1,
	QNM_TF_LEVEL_2,
	QNM_TF_LEVEL_3,
	QNM_TF_LEVEL_4,
	QNM_TF_LEVEL_5,
	QNM_TF_LEVEL_6,
	QNM_TF_LEVEL_7,
	QNM_TF_LEVEL_8,
	QNM_TF_LEVEL_MAX,
}QMA_TF_PARAM_LEVEL;

//setting FM ch step
#define QNM_FM_CH_STEP_50KHZ             5
#define QNM_FM_CH_STEP_100KHZ            10
#define QNM_FM_CH_STEP_200KHZ            20

// FMR start ,stop
#define QNM_FM_CH_START01                8750
#define QNM_FM_CH_START02                7600

#define QNM_FM_CH_STOP01                 10800
#define QNM_FM_CH_STOP02                 10790
#define QNM_FM_CH_STOP03                 9000

//setting test mode 
#define QNM_TEST_PATTERN                 0
#define QNM_TEST_TONE                    1

#define QNM_CH_DECI                      1048576        
#define QNM_FREQ_DECI                    65536
#define QNM_P_MULTI                      100
#define QNM_UHF_START_FREQ               47125
#define QNM_CHN_BW_MASK                  0x0c
#define QNM_P_BIAS                       125
#define QNM_BCLAMP_SYNC_VALUE            0x92

// setting crystal coefficient
#define QNM_CRYSTAL_TH                   20000000
#define QNM_KHZ_UNIT                     1000
#define QNM_CONST_COEFF1                 3456
#define QNM_CONST_COEFF2                 1728
#define QNM_CONST_CLKDIV_COEFF1          (QNM_CONST_COEFF1*QNM_FREQ_DECI)
#define QNM_CONST_CLKDIV_COEFF2          (QNM_CONST_COEFF2*QNM_FREQ_DECI)

#define QNM_CRYSTAL_LOWTH                32768
#define QNM_CRYSTAL_MIDTH                12000000
#define QNM_CRYSTAL_HIGHTH               38400000

#define QNM_CONST_COEFF3                 417792
#define QNM_CONST_COEFF4                 2700*512

#define QNM_10KHZ_UNIT                   10000

#define CRYSTAL 32768
#if(CRYSTAL == 32768)
#define CLK_DLT_MSB                      0x78   
#define CLK_DLT_LSB                      0xf3
#endif     

//seeking channel algorithm
#define QNM_FREQ_BIAS                    125

#define QNM_FREQ_BAND_MAX                11
#define QNM_IMR_NUM                      5
#define QNM_READ_RSSI_DELAY              10

#define QNM_CHN_BW_6MHZ                  600
#define QNM_CHN_BW_7MHZ                  700
#define QNM_CHN_BW_8MHZ                  800

#define QNM_CONFIG_RX_REQ\
        QNF_SetData(QNR_SYSTEM1_RX_REQ,QNR_RX_REQ_ENTER)
#define QNM_CONFIG_FM_MODE\
        QNF_SetData(QNR_SYSTEM1_RX_MODE,QNR_RX_MODE_FM)
#define QNM_CONFIG_ATV_MODE\
	    QNF_SetData(QNR_SYSTEM1_RX_MODE,QNR_RX_MODE_ATV)
#define QNM_CONFIG_ATV_NTSC_STD\
        QNF_SetData(QNR_TV_STD1_VID_STD,QNR_TV_STD1_VID_STD_NTSC)
#define QNM_CONFIG_ATV_PALM_STD\
        QNF_SetData(QNR_TV_STD1_VID_STD,QNR_TV_STD1_VID_STD_PALM)
#define QNM_CONFIG_ATV_PALD_STD\
        QNF_SetData(QNR_TV_STD1_VID_STD,QNR_TV_STD1_VID_STD_PALD)
#define QNM_CONFIG_ATV_SECAM_STD\
        QNF_SetData(QNR_TV_STD1_VID_STD,QNR_TV_STD1_VID_STD_SECAM)
#define QNM_TEST_PATTERN_ENABLE \
        QNF_SetData(QNR_VID_CTRL1_VID_SRC,QNR_VID_SRC_TEST_PATTERN)
#define QNM_TEST_PATTERN_DISABLE \
        QNF_SetData(QNR_VID_CTRL1_VID_SRC,QNR_VID_SRC_ATV_DECODER)
#define QNM_TEST_TONE_ENABLE\
        QNF_SetData(QNR_SYSTEM2_AUD_TEST,QNR_AUD_TEST)
#define QNM_TEST_TONE_DISABLE\
        QNF_SetData(QNR_SYSTEM2_AUD_TEST,QNR_AUD_NORMAL)
#define QNM_TEST_TONE_FREQ(value)\
	    QNF_SetData(0xe4ff,value)
#define QNM_BITSAP_MLSB\
	    QNF_SetData(QNR_VID_CTRL2_BIT_SWAP,QNR_BIT_SWAP_MLSB)
#define QNM_BITSAP_LMSB\
	    QNF_SetData(QNR_VID_CTRL2_BIT_SWAP,QNR_BIT_SWAP_LMSB)
#define QNM_GET_TV_RSSI\
	    QNF_GetData(0xcbff)
#define QNM_GET_FM_RSSI\
	    QNF_GetData(0xe3ff)
#define QNM_ConfigATVChnBandWidth(val)\
	    QNF_SetData(QNR_TV_STD1_CHN_BW,val)
#define QNM_ConfigATVAudioCarrier(val)\
	    QNF_SetData(QNR_TV_STD1_AUD_CAR,val)

//PCLK, H/Vsync polarity and color order YCrCb
#if 1//UPDATE_DRIVER
#define QNM_CFG_HSYNC_HIGH                QNF_SetData(QNR_VID_CTRL2_HS_POL,QNR_HS_POL_NOT_INVERT)
#define QNM_CFG_HSYNC_LOW                 QNF_SetData(QNR_VID_CTRL2_HS_POL,QNR_HS_POL_INVERT)
#define QNM_CFG_VSYNC_HIGH	              QNF_SetData(QNR_VID_CTRL2_VS_POL,QNR_VS_POL_NOT_INVERT)
#define QNM_CFG_VSYNC_LOW	                QNF_SetData(QNR_VID_CTRL2_VS_POL,QNR_VS_POL_INVERT)
#define QNM_CFG_COLOR_ORDER_Y1Cb1Y2Cr1    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_YCbYCr)
#define QNM_CFG_COLOR_ORDER_Y1Cr1Y2Cb1    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_YCrYCb)
#define QNM_CFG_COLOR_ORDER_Cb1Y1Cr1Y2    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_CbYCrY)
#define QNM_CFG_COLOR_ORDER_Cr1Y1Cb1Y2    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_CrYCbY)
#define QNM_CFG_PCLK_HIGH                 QNF_SetData(QNR_VID_CTRL2_PCLKPOL,QNR_PCLKPOL_NOT_INVERT)
#define QNM_CFG_PCLK_LOW                  QNF_SetData(QNR_VID_CTRL2_PCLKPOL,QNR_PCLKPOL_INVERT)
#else
#define QNM_CFG_HSYNC_LOW                 QNF_SetData(QNR_VID_CTRL2_HS_POL,QNR_HS_POL_NOT_INVERT)
#define QNM_CFG_HSYNC_HIGH                QNF_SetData(QNR_VID_CTRL2_HS_POL,QNR_HS_POL_INVERT)
#define QNM_CFG_VSYNC_LOW	              QNF_SetData(QNR_VID_CTRL2_VS_POL,QNR_VS_POL_NOT_INVERT)
#define QNM_CFG_VSYNC_HIGH	              QNF_SetData(QNR_VID_CTRL2_VS_POL,QNR_VS_POL_INVERT)
#define QNM_CFG_COLOR_ORDER_Y1Cb1Y2Cr1    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_YCbYCr)
#define QNM_CFG_COLOR_ORDER_Y1Cr1Y2Cb1    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_YCrYCb)
#define QNM_CFG_COLOR_ORDER_Cb1Y1Cr1Y2    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_CbYCrY)
#define QNM_CFG_COLOR_ORDER_Cr1Y1Cb1Y2    QNF_SetData(QNR_VID_CTRL2_ORDER,QNR_ORDER_CrYCbY)
#define QNM_CFG_PCLK_LOW                  QNF_SetData(QNR_VID_CTRL2_PCLKPOL,QNR_PCLKPOL_NOT_INVERT)
#define QNM_CFG_PCLK_HIGH                 QNF_SetData(QNR_VID_CTRL2_PCLKPOL,QNR_PCLKPOL_INVERT)
#endif
// driving stength
#define QNM_CFG_PCLK_DRV_STRENGTH_LOW     QNF_SetData(QNR_PCLK_CTRL_PCLK_DRV,QNR_PCLK_DRV_LOW) 
#define QNM_CFG_PCLK_DRV_STRENGTH_MIDDLE  QNF_SetData(QNR_PCLK_CTRL_PCLK_DRV,QNR_PCLK_DRV_MIDDLE) 
#define QNM_CFG_PCLK_DRV_STRENGTH_HIGH    QNF_SetData(QNR_PCLK_CTRL_PCLK_DRV,QNR_PCLK_DRV_HIGH) 
#define QNM_CFG_PCLK_DRV_STRENGTH_HIGEST  QNF_SetData(QNR_PCLK_CTRL_PCLK_DRV,QNR_PCLK_DRV_HIGEST)
#define QNM_CFG_DATA_DRV_STRENGTH_LOW     QNF_SetData(QNR_PCLK_CTRL_DOUT_DRV,QNR_DOUT_DRV_LOW)
#define QNM_CFG_DATA_DRV_STRENGTH_MIDDLE  QNF_SetData(QNR_PCLK_CTRL_DOUT_DRV,QNR_DOUT_DRV_MIDDLE)
#define QNM_CFG_DATA_DRV_STRENGTH_HIGH    QNF_SetData(QNR_PCLK_CTRL_DOUT_DRV,QNR_DOUT_DRV_HIGH)
#define QNM_CFG_DATA_DRV_STRENGTH_HIGEST  QNF_SetData(QNR_PCLK_CTRL_DOUT_DRV,QNR_DOUT_DRV_HIGEST)

//video output format,regardless of TV system
#define QNM_SetVideoOutEnable(enable)\
	    QNF_SetData(QNR_VID_CTRL1_VID_EN,enable)
#define QNM_SetVideoITUMode(mode)\
	    QNF_SetData(QNR_VID_CTRL1_ITU_MODE,mode)
#define QNM_SetYUVScale(scale)\
	    QNF_SetData(QNR_VID_CTRL2_YUVRANGE,scale)
#define QNM_SetChromaLumaOrder(val)\
	    QNF_SetData(0x8f60,val)
#define QNM_SetVidOutType(vidOutType)\
	    QNF_SetData(QNR_VID_CTRL1_CLR_TYPE,vidOutType)
#define QNM_GetATVAudMode()\
	    QNF_GetData(QNR_STATUS2_AUD_MODE)

//I2S
#define QNM_SetAnalgAndDigitalAudioOut(audio)\
	    QNF_SetData(QNR_SYSTEM2_I2S_EN,audio?QNR_SYSTEM2_DIGITAL_AUDIO:QNR_SYSTEM2_ANALOG_AUDIO)
#define QNM_ConfigI2SClockPolarity(val)\
	    QNF_SetData(QNR_AUD_CTRL2_I2S_SCKINV,val)
#define QNM_GetI2SBufferState()\
	    QNF_GetData(QNR_I2S_BUFFER_STA)
#define QNM_ConfigI2SBitWidth(val)\
	    QNF_SetData(QNR_I2SBW,val)
#define QNM_ConfigI2SMaster(val)\
	    QNF_SetData(QNR_I2SMASTER,val)
#define QNM_ConfigI2SMode(mode)\
	    QNF_SetData(QNR_I2SMODE,val)

//driving strength
#define QNM_ConfigDSADrvStrength(val)\
	    QNF_SetData(QNR_SDA_DRV,val)
#define QNM_ConfigPCLKState(val)\
	    QNF_SetData(QNR_PCLK_CTRL_PCLK_CTRL,val)
#define QNM_ConfigDataState(val)\
	    QNF_SetData(QNR_PCLK_CTRL_DOUT_CTRL,val)

//EQ
#define QNM_SetEQ(val)\
	    QNF_SetData(QNR_AUD_CTRL1_AEQ_BP,val)
#define QNM_ConfigEQQF(val)\
	    QNF_SetData(QNR_AUD_CTRL2_Q_BS,val)
#define QNM_ConfigEQFC(val)\
	    QNF_SetData(QNR_AUD_CTRL3_FC_BS,val)
#define QNM_ConfigEQGain(val)\
	    QNF_SetData(QNR_AUD_CTRL3_GAIN_BS,val)

extern const UINT32  Nbw[33];

extern void QNF_SetData(UINT16 reg,UINT8 val);
extern UINT8 QNF_GetData(UINT16 reg);
extern void QNF_SetIMR(S_MA_DRIVER *qma,UINT8 imr);
extern void QNF_SetTVBW(S_MA_DRIVER *qma,UINT8 tvBW);
extern void QNF_InitVariable(S_MA_DRIVER *qma);
extern UINT32 QNF_GetCHFreq(S_MA_DRIVER *qma, UINT8 chNo);
extern UINT8 QNF_GetCHNo(S_MA_DRIVER *qma, UINT8 index);
extern void QNF_InitChip(S_MA_DRIVER *qma,UINT8 dev);
extern UINT8 QNF_GetRgnChCount(S_MA_DRIVER *qma);
extern UINT8 QNF_GetATVIndex(S_MA_DRIVER *qma,UINT32 chfreq);
extern UINT32 QNF_GetATVChannel(S_MA_DRIVER *qma,UINT32 chFreq);
extern UINT8 QNF_SetSysClock2(S_MA_DRIVER *qma, UINT8 clkSource, UINT8 clkWaveform, UINT32 freq);
extern UINT8 QNF_GetRxState(S_MA_DRIVER *qma);
extern UINT8 QNF_SetNF1(S_MA_DRIVER *qma,UINT32 freq,UINT16 p);
extern UINT8 QNF_SetNF2(S_MA_DRIVER *qma,UINT32 crystal, UINT32 freq,UINT16 p);
extern UINT32 QNF_GetInteger(UINT32 freq,UINT16 p,UINT8 n,UINT8 m);
extern UINT32 QNF_GetRemainder(UINT32 freq,UINT16 p,UINT8 n,UINT8 m);
extern UINT8 QNF_SetATVCH(S_MA_DRIVER *qma, UINT32 freq);
extern UINT8 QNF_ATVSeekChCond(S_MA_DRIVER *qma);
extern void QNF_PatchBG(S_MA_DRIVER *qma);
extern void QNF_PatchClrField(S_MA_DRIVER *qma);
extern void QNF_AudioAutoDetction(S_MA_DRIVER *qma);
extern void QNF_AudioAutoDetction1(S_MA_DRIVER *qma,UINT32 freq);
extern void QNF_SetATVStd(S_MA_DRIVER *qma, UINT8 tvstd);
extern void QNF_ATVAudDecSel(S_MA_DRIVER *qma, UINT8 tvstd);
extern void QNF_SetGPIOEn(S_MA_DRIVER *qma, UINT16 gpio,UINT8 option);
extern void QNF_SetGPIOData(S_MA_DRIVER *qma, UINT16 gpio,UINT8 data);
extern void QNF_SetGPIO01Select(S_MA_DRIVER *qma, UINT16 gpio,UINT8 data);
extern UINT8 QNF_GetGPIOStatus(S_MA_DRIVER *qma,UINT16 gpio);
extern UINT16 QNF_GetATVBW(S_MA_DRIVER *qma);
extern void QNF_PowerDown(S_MA_DRIVER *qma);
extern void QNF_RestartFromPowerDown(S_MA_DRIVER *qma);

//video
extern void QNF_InitConfigVideo(S_MA_DRIVER *qma);
extern void QNF_ConfigResolution(S_MA_DRIVER *qma,UINT8 size);
extern UINT8 QNF_ConfigTFParam(S_MA_DRIVER *qma, QMA_TF_PARAM_LEVEL val);
extern void QNF_ConfigVideoOutputFormat(S_MA_DRIVER *qma,UINT8 ituMode);
extern void QNF_SetActiveStartPxlNum(S_MA_DRIVER *qma,UINT8 pixelNum);
extern void QNF_SetActivePxlNum(S_MA_DRIVER *qma,UINT16 pixelNum);
extern void QNF_SetActiveStartLineNum1(S_MA_DRIVER *qma,UINT16 lineNum1);
extern void QNF_SetActiveStartLineNum2(S_MA_DRIVER *qma,UINT16 lineNum2);
extern void QNF_SetActiveLineNum(S_MA_DRIVER *qma,UINT16 lineNum);
extern void QNF_SetVsyncStartLineNum1(S_MA_DRIVER *qma,UINT16 lineNum1);
extern void QNF_SetVsyncStopLineNum1(S_MA_DRIVER *qma,UINT16 lineNum1);
extern void QNF_SetVsyncStartLineNum2(S_MA_DRIVER *qma,UINT16 lineNum2);
extern void QNF_SetVsyncStopLineNum2(S_MA_DRIVER *qma,UINT16 lineNum2);
extern UINT8 QNF_CheckSyncDetectState();
extern void QNF_SetBlackWhiteMode(S_MA_DRIVER *qma,UINT8 val);
//audio for ATV
extern void QNF_InitConfigAudio(S_MA_DRIVER *qma);
extern void QNF_SetAudMuteEnable(S_MA_DRIVER *qma,UINT8 on);
extern void QNF_SetDeemp(UINT8 on, UINT8 emp_val);
extern void QNF_SetTVAudSTD( UINT8 tvAudSTD);
extern void QNF_SetTVAudSelect(UINT8 audSel);
extern void QNF_SetAudioGain(S_MA_DRIVER *qma,UINT8 val);
extern void QNF_SetAudioVolume(S_MA_DRIVER *qma,UINT8 val);
extern UINT8 QNF_SetSysClock(S_MA_DRIVER *qma, UINT8 clkSource, UINT8 clkWaveform, UINT32 clkFrequency);
extern void QNF_TVAutoDetection(S_MA_DRIVER *qma);
extern void QNF_TVCkillSelection(S_MA_DRIVER *qma,UINT8 option);
#endif //_QMAFUNC_H_