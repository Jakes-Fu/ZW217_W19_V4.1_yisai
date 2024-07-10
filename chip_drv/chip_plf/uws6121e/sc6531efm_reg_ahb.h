/**----------------------------------------------------------------------------*
 ** File Name:    sc6531efm_reg_ahb.h                                             *
 ** Author:       Windy.Wang                                                   *
 ** DATE:         2012-3-22                                                    *
 ** Copyright:    2012 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                               *
 **----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
 **                   Edit    History                                          *
 **----------------------------------------------------------------------------*
 ** DATE          NAME                 DESCRIPTION                             *
 ** 2012-03-22    Windy.Wang           Create.                                 *
 ** 2012-04-12    windy.wang           Modified.                               *
 ** 2012-09-25    windy.wang           Create base on 2012.5.22 document       *
 **----------------------------------------------------------------------------*/


#ifndef _SC6531EFM_REG_AHB_H_
#define _SC6531EFM_REG_AHB_H_


/**----------------------------------------------------------------------------*
 **                      Compiler Flag                                         *
 **----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#define    AHB_RST0_SET       (AHB_REG_BASE + 0x1040)
#define    AHB_RST0_CLR       (AHB_REG_BASE + 0x2040)
#define    AHB_RST0           (AHB_REG_BASE + 0x0040)
#define    AHB_EB0_SET        (AHB_REG_BASE + 0x1080)
#define    AHB_EB0_CLR        (AHB_REG_BASE + 0x2080)
#define    AHB_EB0            (AHB_REG_BASE + 0x0080)
#define    AHB_ARCH_EB_REG    (AHB_REG_BASE + 0x0084)
#define    AHB_ARM9_CORE_RST_FLAG    (AHB_REG_BASE + 0x0088)
#define    AHB_BUS_CTL0       (AHB_REG_BASE + 0x00A0)
#define    AHB_BUS_CTL1       (AHB_REG_BASE + 0x00A4)
#define    AHB_BUS_CTL2       (AHB_REG_BASE + 0x00A8)
#define    AHB_BUS_CTL3       (AHB_REG_BASE + 0x00AC)
#define    AHB_SYS_CTL0       (AHB_REG_BASE + 0x00B0)
#define    AHB_SYS_CTL1       (AHB_REG_BASE + 0x00B4)
#define    AHB_SYS_CTL2       (AHB_REG_BASE + 0x00B8)
#define    AHB_SYS_CTL3       (AHB_REG_BASE + 0x00BC)
#define    AHB_SYS_CTL4       (AHB_REG_BASE + 0x00C0)
#define    AHB_SYS_CTL5       (AHB_REG_BASE + 0x00C4)
#define    AHB_SLP_CTL0       (AHB_REG_BASE + 0x00D0)
#define    AHB_SLP_CTL1       (AHB_REG_BASE + 0x00D4)
#define    AHB_SLP_CTL2       (AHB_REG_BASE + 0x00D8)
#define    AHB_SYS_CTL6       (AHB_REG_BASE + 0x00E0)
#define    AHB_SYS_CTL7       (AHB_REG_BASE + 0x00E4)
#define    AHB_SYS_CTL8       (AHB_REG_BASE + 0x00E8)
#define    AHB_SYS_CTL9       (AHB_REG_BASE + 0x00EC)
#define    AHB_SYS_CTL10      (AHB_REG_BASE + 0x00F0)
#define    AHB_SYS_CTL11      (AHB_REG_BASE + 0x00F4)
#define    AHB_SLP_STS0       (AHB_REG_BASE + 0x0100)
#define    AHB_SLP_DBG       (AHB_REG_BASE + 0x0104)
#define    AHB_FUNC_TST_ADDR       (AHB_REG_BASE + 0x0108)
#define    AHB_ARCH_PORT      (AHB_REG_BASE + 0x0120)
#define    AHB_SYS_CTL12      (AHB_REG_BASE + 0x0124)
#define    AHB_SYS_CTL13      (AHB_REG_BASE + 0x0128)
#define    AHB_SYS_CTL14      (AHB_REG_BASE + 0x012C)
#define    AHB_SYS_CTL15      (AHB_REG_BASE + 0x0130)
#define    AHB_SYS_CTL16      (AHB_REG_BASE + 0x0134)
#define    AHB_SYS_CTL17      (AHB_REG_BASE + 0x0138)
#define    AHB_SYS_CTL18      (AHB_REG_BASE + 0x013C)
#define    AHB_SYS_CTL19      (AHB_REG_BASE + 0x0140)
#define    AHB_SYS_CTL20      (AHB_REG_BASE + 0x0144)
#define    AHB_SYS_CTL21      (AHB_REG_BASE + 0x0148)
#define    AHB_SYS_CTL22      (AHB_REG_BASE + 0x014C)
#define    AHB_SYS_CTL23      (AHB_REG_BASE + 0x0150)
#define    AHB_DMA_SLP_CTL2   (AHB_REG_BASE + 0x0154) 
#define    AHB_DAP_AB_CTL     (AHB_REG_BASE + 0x0158)
#define    AHB_SENSOR_MODE    (AHB_REG_BASE + 0x0160)
#define    AHB_PTEST_BOOT_ADDR    (AHB_REG_BASE + 0x0164)
#define    AHB_CHIP_ID        (AHB_REG_BASE + 0x03FC)

/*******************************************************************************
 ** Register Name       : AHB_RST0_SET
 ** Register Offset     : 0x1040
 ** Description         : The AHB soft reset set control registers 0. 
 ** Reserved             [31:20]
 ** BTRF_SOFT_RST_SET    [19:19] BT RF soft reset set bit
 ** FMRF_SOFT_RST_SET    [18:18] FM RF soft reset set bit
 ** ADDA_SOFT_RST_SET    [17:17] ADDA soft reset set bit
 ** RFCTRL_SOFT_RST_SET    [16:16] RF controller soft reset set bit
 ** SPI1_SOFT_RST_SET    [15:15] SPI1 controller soft reset set bit
 ** ZIP0_SOFT_RST_SET    [13:13] ZIP0 controller soft reset set bit
 ** ROTA_SOFT_RST_SET    [12:12] Rotation controller soft reset set bit
 ** SPI0_SOFT_RST_SET    [11:11] SPI0 controller soft reset set bit
 ** LCDC_SOFT_RST_SET    [9 :9 ] LCDC controller soft reset set bit
 ** SDIO_SOFT_RST_SET    [8 :8 ] SDIO controller soft reset set bit
 ** SFC_SOFT_RST_SET     [7 :7 ] Serial flash controller soft reset set bit
 ** LCM_SOFT_RST_SET     [6 :6 ] LCM controller soft reset set bit
 ** EMC_SOFT_RST_SET     [5 :5 ] EMC soft reset set bit
 ** USBD_SOFT_RST_SET    [4 :4 ] USB device controller soft reset set bit
 ** VSP_SOFT_RST_SET     [3 :3 ] VSP soft reset set bit
 ** CCIR_SOFT_RST_SET    [2 :2 ] CCIR soft reset set bit
 ** DCAM_SOFT_RST_SET    [1 :1 ] DCAM soft reset set bit
 ** DMA_SOFT_RST_SET     [0 :0 ] DMAsoft reset set bit
 ******************************* Field definition ******************************/
#define    BTRF_SOFT_RST_SET          (BIT_19)
#define    FMRF_SOFT_RST_SET          (BIT_18)
#define    ADDA_SOFT_RST_SET         (BIT_17)
#define    RFCTRL_SOFT_RST_SET      (BIT_16)
#define    SPI1_SOFT_RST_SET          (BIT_15)
#define    ZIP0_SOFT_RST_SET          (BIT_13)
#define    ROTA_SOFT_RST_SET          (BIT_12)
#define    SPI0_SOFT_RST_SET          (BIT_11)
#define    LCDC_SOFT_RST_SET          (BIT_9)
#define    SDIO_SOFT_RST_SET          (BIT_8)
#define    SFC_SOFT_RST_SET           (BIT_7)
#define    LCM_SOFT_RST_SET           (BIT_6)
#define    EMC_SOFT_RST_SET           (BIT_5)
#define    USBD_SOFT_RST_SET          (BIT_4)
#define    VSP_SOFT_RST_SET           (BIT_3)
#define    CCIR_SOFT_RST_SET          (BIT_2)
#define    DCAM_SOFT_RST_SET          (BIT_1)
#define    DMA_SOFT_RST_SET           (BIT_0)
#define    ZIP_SOFT_RST_SET           ZIP0_SOFT_RST_SET

/*******************************************************************************
 ** Register Name       : AHB_RST0_CLR
 ** Register Offset     : 0x2040
 ** Description         : The AHB soft reset clear control registers 0. 
 ** Reserved             [31:20]
 ** BTRF_SOFT_RST_CLR    [19:19] BT RF soft reset clear bit
 ** FMRF_SOFT_RST_CLR    [18:18] FM RF soft reset clear bit
 ** ADDA_SOFT_RST_CLR    [17:17] ADDA soft reset clear bit
 ** RFCTRL_SOFT_RST_CLR    [16:16] RF controller soft reset clear bit
 ** SPI1_SOFT_RST_CLR    [15:15] SPI1 controller soft reset clear bit
 ** ZIP0_SOFT_RST_CLR    [13:13] ZIP0 controller soft reset clear bit
 ** ROTA_SOFT_RST_CLR    [12:12] Rotation controller soft reset clear bit
 ** SPI0_SOFT_RST_CLR    [11:11] SPI0 controller soft reset clear bit
 ** LCDC_SOFT_RST_CLR    [9 :9 ] LCDC controller soft reset clear bit
 ** SDIO_SOFT_RST_CLR    [8 :8 ] Serial flash controller soft reset set bit
 ** SFC_SOFT_RST_CLR     [7 :7 ] Serial flash controller soft reset clear bit
 ** LCM_SOFT_RST_CLR     [6 :6 ] LCM controller soft reset clear bit
 ** EMC_SOFT_RST_CLR     [5 :5 ] EMC soft reset set bit
 ** USBD_SOFT_RST_CLR    [4 :4 ] USB device controller soft reset clear bit
 ** VSP_SOFT_RST_CLR     [3 :3 ] VSP soft reset clear bit
 ** CCIR_SOFT_RST_CLR    [2 :2 ] CCIR soft reset clear bit
 ** DCAM_SOFT_RST_CLR    [1 :1 ] DCAM soft reset set bit
 ** DMA_SOFT_RST_CLR     [0 :0 ] DMAsoft reset cleart bit
 ******************************* Field definition ******************************/
#define    BTRF_SOFT_RST_CLR          (BIT_19)
#define    FMRF_SOFT_RST_CLR          (BIT_18)
#define    ADDA_SOFT_RST_CLR         (BIT_17)
#define    RFCTRL_SOFT_RST_CLR      (BIT_16)
#define    SPI1_SOFT_RST_CLR          (BIT_15)
#define    ZIP0_SOFT_RST_CLR          (BIT_13)
#define    ROTA_SOFT_RST_CLR          (BIT_12)
#define    SPI0_SOFT_RST_CLR          (BIT_11)
#define    LCDC_SOFT_RST_CLR          (BIT_9)
#define    SDIO_SOFT_RST_CLR          (BIT_8)
#define    SFC_SOFT_RST_CLR           (BIT_7)
#define    LCM_SOFT_RST_CLR           (BIT_6)
#define    EMC_SOFT_RST_CLR           (BIT_5)
#define    USBD_SOFT_RST_CLR          (BIT_4)
#define    VSP_SOFT_RST_CLR           (BIT_3)
#define    CCIR_SOFT_RST_CLR          (BIT_2)
#define    DCAM_SOFT_RST_CLR          (BIT_1)
#define    DMA_SOFT_RST_CLR           (BIT_0)
#define    ZIP_SOFT_RST_CLR           ZIP0_SOFT_RST_CLR

/*******************************************************************************
 ** Register Name       : AHB_RST0
 ** Register Offset     : 0x0040
 ** Description         : The AHB soft reset status control registers 0. 
 ** Reserved         [31:20]
 ** BT_RF_SOFT_RST   [19:19] BT_RF soft reset status
 ** FM_RF_SOFT_RST   [18:18] FM_RF soft reset status
 ** ADDA_SOFT_RST    [17:17] ADDA soft reset status
 ** RF_CTRL_SOFT_RST [16:16] RF_CTRL soft reset status
 ** SPI1_SOFT_RST    [15:15] SPI1 soft reset status
 ** RESERVED         [14:14] 
 ** ZIP0_SOFT_RST    [13:13] ZIP0 soft reset status
 ** ROTA_SOFT_RST    [12:12] ROTA soft reset status
 ** SPI0_SOFT_RST    [11:11] SPI0 soft reset status
 ** RESERVED    [10:10] 
 ** LCDC_SOFT_RST    [9 :9 ] LCDC soft reset status
 ** SDIO_SOFT_RST    [8 :8 ] SDIO soft reset status
 ** SFC_SOFT_RST     [7 :7 ] Serial flash controller soft reset status
 ** LCM_SOFT_RST     [6 :6 ] LCM controller soft reset status
 ** EMC_SOFT_RST     [5 :5 ] EMC soft reset status
 ** USBD_SOFT_RST    [4 :4 ] USB device controller soft reset 
 ** VSP_SOFT_RST     [3 :3 ] VSP soft reset status
 ** CCIR_SOFT_RST    [2 :2 ] CCIR soft reset status
 ** DCAM_SOFT_RST    [1 :1 ] DCAM soft reset status
 ** DMA_SOFT_RST     [0 :0 ] DMAsoft reset status
 ******************************* Field definition ******************************/
#define    BT_RF_SOFT_RST         (BIT_19)
#define    FM_RF_SOFT_RST         (BIT_18)  
#define    ADDA_SOFT_RST          (BIT_17)  
#define    RF_CTRL_SOFT_RST       (BIT_16)    
#define    SPI1_SOFT_RST          (BIT_15)
#define    ZIP0_SOFT_RST          (BIT_13)
#define    ROTA_SOFT_RST          (BIT_12)
#define    SPI0_SOFT_RST          (BIT_11)
#define    LCDC_SOFT_RST          (BIT_9)
#define    SDIO_SOFT_RST          (BIT_8)
#define    SFC_SOFT_RST           (BIT_7)
#define    LCM_SOFT_RST           (BIT_6)
#define    EMC_SOFT_RST           (BIT_5)
#define    USBD_SOFT_RST          (BIT_4)
#define    VSP_SOFT_RST           (BIT_3)
#define    CCIR_SOFT_RST          (BIT_2)
#define    DCAM_SOFT_RST          (BIT_1)
#define    DMA_SOFT_RST           (BIT_0)
#define    ZIP_SOFT_RST           ZIP0_SOFT_RST

/*******************************************************************************
 ** Register Name       : AHB_EB0_SET
 ** Register Offset     : 0x1080
 ** Description         : The AHB module enabel set control register0
 ** Reserved            [31:20]
 ** SPI1_EB_SET      [19:19] SPI1 enable set bit
 ** Reserved            [18:18]
 ** Reserved            [17:17]
 ** ZIP0_EB_SET      [16:16] ZIP0 enable set bit
 ** ROTA_EB_SET     [15:15] ROTA enable set bit
 ** SPI0_EB_SET      [14:14] SP10 enable set bit
 ** Reserved            [13:13] 
 ** LCDC_EB_SET    [12:12] LCDC enable set bit
 ** Reserved            [11:11] 
 ** Reserved            [10:10]
 ** SDIO_EB_SET     [9 :9 ] SDIO enable set bit
 ** EMC_EB_SET      [8 :8 ] EMC enable set bit
 ** SFC_EB_SET       [7 :7 ] Seiral flash controller enable set bit
 ** LCM_EB_SET       [6 :6 ] LCM controller enable set bit
 ** Reserved            [5 :5 ] 
 ** USBD_EB_SET             [4 :4 ] USB device controller enable set bit
 ** Reserved                     [3 :3 ] 
 ** BUSMON0_EB_SET       [2 :2 ] AHB bus moniror 0 enable set bit
 ** DCAM_EB_SET             [1 :1 ] DCAM enable set bit
 ** DMA_EB_SET               [0 :0 ] DMA enable set bit
 ******************************* Field definition ******************************/
#define    SPI1_EB_SET             (BIT_19)
#define    ZIP0_EB_SET             (BIT_16)
#define    ROTA_EB_SET             (BIT_15)
#define    SPI0_EB_SET             (BIT_14)
#define    LCDC_EB_SET             (BIT_12)
#define    SDIO_EB_SET             (BIT_9)
#define    EMC_EB_SET              (BIT_8)
#define    SFC_EB_SET              (BIT_7)
#define    LCM_EB_SET              (BIT_6)
#define    USBD_EB_SET             (BIT_4)
#define    BUSMON0_EB_SET          (BIT_2)
#define    DCAM_EB_SET             (BIT_1)
#define    DMA_EB_SET              (BIT_0)
#define    ZIP_EB_SET              ZIP0_EB_SET

/*******************************************************************************
 ** Register Name       : AHB_EB0_CLR
 ** Register Offset     : 0x2080
 ** Description         : The AHB module enabel clear control register 0
 ** Reserved                [31:20]
 ** SPI1_EB_CLR         [19:19] SPI1 enable clear bit
 ** Reserved                [18:18] 
 ** Reserved                [17:17] 
 ** ZIP0_EB_CLR         [16:16] ZIP0 enable clear bit
 ** ROTA_EB_CLR        [15:15] ROTA enable clear bit
 ** SPI0_EB_CLR         [14:14] SP10 enable clear bit
 ** MIDI_EB_CLR         [13:13] MIDI enable clear bit
 ** LCDC_EB_CLR        [12:12] LCDC enable clear bit
 ** Reserved                [11:11] 
 ** Reserved                [10:10]
 ** SDIO_EB_CLR         [9 :9 ] SDIO enable clear bit
 ** EMC_EB_CLR          [8 :8 ] EMC enable clear bit
 ** SFC_EB_CLR          [7 :7 ] Seiral flash controller enable clear bit
 ** LCM_EB_CLR          [6 :6 ] LCM controller enable clear bit
 ** Reserved                [5 :5 ] 
 ** USBD_EB_CLR         [4 :4 ] USB device controller enable clear bit
 ** Reserved                [3 :3 ]
 ** BUSMON0_EB_CLR    [2 :2 ] AHB bus moniror 0 enable clear bit
 ** DCAM_EB_CLR       [1 :1 ] DCAM enable clear bit
 ** DMA_EB_CLR        [0 :0 ] DMA enable clear bit
 ******************************* Field definition ******************************/
#define    SPI1_EB_CLR             (BIT_19)
#define    ZIP0_EB_CLR             (BIT_16)
#define    ROTA_EB_CLR             (BIT_15)
#define    SPI0_EB_CLR             (BIT_14)
#define    LCDC_EB_CLR             (BIT_12)
#define    SDIO_EB_CLR             (BIT_9)
#define    EMC_EB_CLR              (BIT_8)
#define    SFC_EB_CLR              (BIT_7)
#define    LCM_EB_CLR              (BIT_6)
#define    USBD_EB_CLR             (BIT_4)
#define    BUSMON0_EB_CLR          (BIT_2)
#define    DCAM_EB_CLR             (BIT_1)
#define    DMA_EB_CLR              (BIT_0)
#define    ZIP_EB_CLR              ZIP0_EB_CLR

/*******************************************************************************
 ** Register Name       : AHB_EB0
 ** Register Offset     : 0x0080
 ** Description         : The AHB module enabel status
 ** Reserved        [31:20]
 ** SPI1_EB          [19:19] SPI1 enable status bit
 ** Reserved        [18:18] 
 ** Reserved        [17:17] 
 ** ZIP0_EB         [16:16] ZIP0 enable status bit
 ** ROTA_EB       [15:15] ROTA enable status bit
 ** SPI0_EB         [14:14] SP10 enable status bit
 ** Reserved       [13:13] 
 ** LCDC_EB       [12:12] LCDC enable status bit
 ** Reserved      [11:10]
 ** SDIO_EB       [9 :9 ] SDIO Enable status bit
 ** EMC_EB        [8 :8 ] EMC enable status bit
 ** SFC_EB        [7 :7 ] Seiral flash controller enable status bit
 ** LCM_EB        [6 :6 ] LCM controller enable status bit
 ** Reserved        [5 :5 ] 
 ** USBD_EB       [4 :4 ] USB device controller enable status bit
 ** Reserved    [3 :3 ] 
 ** BUSMON0_EB    [2 :2 ] AHB bus moniror 0 enable status bit
 ** DCAM_EB       [1 :1 ] DCAM enable status bit
 ** DMA_EB        [0 :0 ] DMA enable status bit
 ******************************* Field definition ******************************/
#define    SPI1_EB             (BIT_19)
#define    ZIP0_EB             (BIT_16)
#define    ROTA_EB             (BIT_15)
#define    SPI0_EB             (BIT_14)
#define    LCDC_EB             (BIT_12)
#define    SDIO_EB             (BIT_9)
#define    EMC_EB              (BIT_8)
#define    SFC_EB              (BIT_7)
#define    LCM_EB              (BIT_6)
#define    USBD_EB             (BIT_4)
#define    BUSMON0_EB          (BIT_2)
#define    DCAM_EB             (BIT_1)
#define    DMA_EB              (BIT_0)

/*******************************************************************************
 ** Register Name       : ARCH_EB_REG
 ** Register Offset     : 0x0084
 ** Description         : The AHB ARCH enable
 ** Reserved    [31:1 ]
 ** ARCH_EB     [0 :0 ] ARCH enable status bit
 ******************************* Field definition ******************************/
#define    ARCH_EB           (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_ARM9_CORE_RST_FLAG
 ** Register Offset     : 0x0088
 ** Description         : The ARM9 core reset flag
 ** Reserved              [31:1 ]
 ** ARM9_CORE_RST_FLAG    [0 :0 ] ARM9 core reset flag, SW can write 0 to clear 
 **                               this flag
 ******************************* Field definition ******************************/
#define    ARM9_CORE_RST_FLAG          (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_BUS_CTL0
 ** Register Offset     : 0x00A0
 ** Description         : The AHB bus matrix control register 0
 ** Reserved            [31:20]
 ** LCM_PAUSE_HW0_EN    [19:19] LCM pause controller by hardware 0
 ** EMC_PAUSE_HW0_EN    [18:18] EMC pause controller by hardware 0
 ** SFC_PAUSE_HW0_EN    [17:17] SFC pause controller by hardware 0
 ** Reserved            [16:12]
 ** M11_PAUSE_HW0_EN    [11:11] M11 pause of matrix controlled by hardware 0    
 **                             enable
 ** M10_PAUSE_HW0_EN    [10:10] M10 pause of matrix controlled by hardware 0    
 **                             enable
 ** M9_PAUSE_HW0_EN     [9 :9 ] M9 pause of matrix controlled by hardware 0     
 **                             enable
 ** M8_PAUSE_HW0_EN     [8 :8 ] M8 pause of matrix controlled by hardware 0     
 **                             enable
 ** M7_PAUSE_HW0_EN     [7 :7 ] M7 pause of matrix controlled by hardware 0     
 **                             enable
 ** M6_PAUSE_HW0_EN     [6 :6 ] M6 pause of matrix controlled by hardware 0     
 **                             enable
 ** M5_PAUSE_HW0_EN     [5 :5 ] M5 pause of matrix controlled by hardware 0     
 **                             enable
 ** M4_PAUSE_HW0_EN     [4 :4 ] M4 pause of matrix controlled by hardware 0     
 **                             enable
 ** M3_PAUSE_HW0_EN     [3 :3 ] M3 pause of matrix controlled by hardware 0enabl
 **                             e
 ** M2_PAUSE_HW0_EN     [2 :2 ] M2 pause of matrix controlled by hardware 0     
 **                             enable
 ** M1_PAUSE_HW0_EN     [1 :1 ] M1 pause of matrix controlled by hardware 0     
 **                             enable
 ** M0_PAUSE_HW0_EN     [0 :0 ] M0 pause of matrix controlled by hardware 0     
 **                             enable
 ******************************* Field definition ******************************/
#define    LCM_PAUSE_HW0_EN          (BIT_19)
#define    EMC_PAUSE_HW0_EN          (BIT_18)
#define    SFC_PAUSE_HW0_EN          (BIT_17)
#define    M11_PAUSE_HW0_EN          (BIT_11)
#define    M10_PAUSE_HW0_EN          (BIT_10)
#define    M9_PAUSE_HW0_EN           (BIT_9)
#define    M8_PAUSE_HW0_EN           (BIT_8)
#define    M7_PAUSE_HW0_EN           (BIT_7)
#define    M6_PAUSE_HW0_EN           (BIT_6)
#define    M5_PAUSE_HW0_EN           (BIT_5)
#define    M4_PAUSE_HW0_EN           (BIT_4)
#define    M3_PAUSE_HW0_EN           (BIT_3)
#define    M2_PAUSE_HW0_EN           (BIT_2)
#define    M1_PAUSE_HW0_EN           (BIT_1)
#define    M0_PAUSE_HW0_EN           (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_BUS_CTL1
 ** Register Offset     : 0x00A4
 ** Description         : The AHB bus matrix control register 1
 ** Reserved            [31:20]
 ** LCM_PAUSE_HW1_EN    [19:19] LCM pause controller by hardware 1
 ** EMC_PAUSE_HW1_EN    [18:18] EMC pause controller by hardware 1
 ** SFC_PAUSE_HW1_EN    [17:17] SFC pause controller by hardware 1
 ** Reserved            [16:12]
 ** M11_PAUSE_HW1_EN    [11:11] M11 pause of matrix controlled by hardware 1    
 **                             enable
 ** M10_PAUSE_HW1_EN    [10:10] M10 pause of matrix controlled by hardware 1    
 **                             enable
 ** M9_PAUSE_HW1_EN     [9 :9 ] M9 pause of matrix controlled by hardware 1     
 **                             enable
 ** M8_PAUSE_HW1_EN     [8 :8 ] M8 pause of matrix controlled by hardware 1     
 **                             enable
 ** M7_PAUSE_HW1_EN     [7 :7 ] M7 pause of matrix controlled by hardware 1     
 **                             enable
 ** M6_PAUSE_HW1_EN     [6 :6 ] M6 pause of matrix controlled by hardware 1     
 **                             enable
 ** M5_PAUSE_HW1_EN     [5 :5 ] M5 pause of matrix controlled by hardware 1     
 **                             enable
 ** M4_PAUSE_HW1_EN     [4 :4 ] M4 pause of matrix controlled by hardware 1     
 **                             enable
 ** M3_PAUSE_HW1_EN     [3 :3 ] M3 pause of matrix controlled by hardware 1     
 **                             enable
 ** M2_PAUSE_HW1_EN     [2 :2 ] M2 pause of matrix controlled by hardware 1     
 **                             enable
 ** M1_PAUSE_HW1_EN     [1 :1 ] M1 pause of matrix controlled by hardware 1     
 **                             enable
 ** M0_PAUSE_HW1_EN     [0 :0 ] M0 pause of matrix controlled by hardware 1     
 **                             enable
 ******************************* Field definition ******************************/
#define    LCM_PAUSE_HW1_EN          (BIT_19)
#define    EMC_PAUSE_HW1_EN          (BIT_18)
#define    SFC_PAUSE_HW1_EN          (BIT_17)
#define    M11_PAUSE_HW1_EN          (BIT_11)
#define    M10_PAUSE_HW1_EN          (BIT_10)
#define    M9_PAUSE_HW1_EN           (BIT_9)
#define    M8_PAUSE_HW1_EN           (BIT_8)
#define    M7_PAUSE_HW1_EN           (BIT_7)
#define    M6_PAUSE_HW1_EN           (BIT_6)
#define    M5_PAUSE_HW1_EN           (BIT_5)
#define    M4_PAUSE_HW1_EN           (BIT_4)
#define    M3_PAUSE_HW1_EN           (BIT_3)
#define    M2_PAUSE_HW1_EN           (BIT_2)
#define    M1_PAUSE_HW1_EN           (BIT_1)
#define    M0_PAUSE_HW1_EN           (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_BUS_CTL2
 ** Register Offset     : 0x00A8
 ** Description         : The AHB bus matrix control register 2
 ** Reserved        [31:20]
 ** LCM_PAUSE_SW    [19:19] LCM pause of matrix controlled by software 
 ** EMC_PAUSE_SW    [18:18] EMC pause of matrix controlled by software 
 ** SFC_PAUSE_SW    [17:17] SFC pause of matrix controlled by software 
 ** Reserved        [16:12]
 ** M11_PAUSE_SW    [11:11] M11 pause of matrix controlled by software 
 ** M10_PAUSE_SW    [10:10] M10 pause of matrix controlled by software 
 ** M9_PAUSE_SW     [9 :9 ] M9 pause of matrix controlled by software 
 ** M8_PAUSE_SW     [8 :8 ] M8 pause of matrix controlled by software 
 ** M7_PAUSE_SW     [7 :7 ] M7 pause of matrix controlled by software 
 ** M6_PAUSE_SW     [6 :6 ] M6 pause of matrix controlled by software 
 ** M5_PAUSE_SW     [5 :5 ] M5 pause of matrix controlled by software 
 ** M4_PAUSE_SW     [4 :4 ] M4 pause of matrix controlled by software 
 ** M3_PAUSE_SW     [3 :3 ] M3 pause of matrix controlled by software 
 ** M2_PAUSE_SW     [2 :2 ] M2 pause of matrix controlled by software 
 ** M1_PAUSE_SW     [1 :1 ] M1 pause of matrix controlled by software 
 ** M0_PAUSE_SW     [0 :0 ] M0 pause of matrix controlled by software 
 ******************************* Field definition ******************************/
#define    LCM_PAUSE_SW          (BIT_19)
#define    EMC_PAUSE_SW          (BIT_18)
#define    SFC_PAUSE_SW          (BIT_17)
#define    M11_PAUSE_SW          (BIT_11)
#define    M10_PAUSE_SW          (BIT_10)
#define    M9_PAUSE_SW           (BIT_9)
#define    M8_PAUSE_SW           (BIT_8)
#define    M7_PAUSE_SW           (BIT_7)
#define    M6_PAUSE_SW           (BIT_6)
#define    M5_PAUSE_SW           (BIT_5)
#define    M4_PAUSE_SW           (BIT_4)
#define    M3_PAUSE_SW           (BIT_3)
#define    M2_PAUSE_SW           (BIT_2)
#define    M1_PAUSE_SW           (BIT_1)
#define    M0_PAUSE_SW           (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_BUS_CTL3
 ** Register Offset     : 0x00AC
 ** Description         : The AHB bus matrix control register 3
 ** Reserved                                                    [31:19]
 ** BRIDGE_ARM9_ARM7_ENA                         [18:18] ARM9 to ARM7 AHB bridge enable
 ** BRIDGE_ARM7_EMC_ENA                           [17:17] ARM7 to ARM9 matrix(EMC) bridge   
 **                                                                                 enable
 ** BRIDGE_ARM7_ARM9_ENA                         [16:16] ARM7 to ARM9 matrix(none-EMC)     
 **                                                                                  bridge enable
  ** Reserved                                                   [15:14]
 ** BRIDGE_BUF_STS_S_CMDFIFO_FULL          [13:13] bridge status register, for debug
 ** BRIDGE_BUF_STS_S_CMDFIFO_EMPTY       [12:12] bridge status register, for debug
 ** BRIDGE_BUF_STS_S_RFIFO_FULL               [11:11] bridge status register, for debug
 ** BRIDGE_BUF_STS_S_RFIFO_EMPTY            [10:10] bridge status register, for debug
 ** BRIDGE_BUF_STS_S_IDLE                          [9 :9 ] bridge status register, for debug
 ** BRIDGE_BUF_STS_M_CMDFIFO_FULL         [8 :8 ] bridge status register, for debug
 ** BRIDGE_BUF_STS_M_CMDFIFO_EMPTY      [7 :7 ] bridge status register, for debug
 ** BRIDGE_BUF_STS_M_RFIFO_FULL             [6 :6 ] bridge status register, for debug
 ** BRIDGE_BUF_STS_M_RFIFO_EMPTY           [5 :5 ] bridge status register, for debug
 ** BRIDGE_BUF_STS_M_IDLE                         [4 :4 ] bridge status register, for debug
 ** BRIDGE_BUF_STS_M_ST                            [3 :2 ] bridge status register, for debug
 ** BRIDGE_BUF_SLEEP_READY                      [1 :1 ] bridge status register, for debug
 ** BRIDGE_BUF_PAUSE_READY                      [0 :0 ] bridge status register, for debug
 ******************************* Field definition ******************************/
#define    BRIDGE_ARM9_ARM7_ENA                    (BIT_18)
#define    BRIDGE_ARM7_EMC_ENA                     (BIT_17)
#define    BRIDGE_ARM7_ARM9_ENA                    (BIT_16)
#define    BRIDGE_BUF_STS_S_CMDFIFO_FULL           (BIT_13)
#define    BRIDGE_BUF_STS_S_CMDFIFO_EMPTY          (BIT_12)
#define    BRIDGE_BUF_STS_S_RFIFO_FULL             (BIT_11)
#define    BRIDGE_BUF_STS_S_RFIFO_EMPTY            (BIT_10)
#define    BRIDGE_BUF_STS_S_IDLE                   (BIT_9)
#define    BRIDGE_BUF_STS_M_CMDFIFO_FULL           (BIT_8)
#define    BRIDGE_BUF_STS_M_CMDFIFO_EMPTY          (BIT_7)
#define    BRIDGE_BUF_STS_M_RFIFO_FULL             (BIT_6)
#define    BRIDGE_BUF_STS_M_RFIFO_EMPTY            (BIT_5)
#define    BRIDGE_BUF_STS_M_IDLE                   (BIT_4)
#define    BRIDGE_BUF_STS_M_ST_SHIFT               (2)
#define    BRIDGE_BUF_STS_M_ST_MSK                 (0x3 << BRIDGE_BUF_STS_M_ST_SHIFT)
#define    BRIDGE_BUF_SLEEP_READY                  (BIT_1)
#define    BRIDGE_BUF_PAUSE_READY                  (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL0
 ** Register Offset     : 0x00B0
 ** Description         : The MCU  system control register 0
 ** Reserved           [31:13]
 ** BUSMON0_CHN2_MUX           [12:12] 
 ** BUSMON0_CHN1_MUX           [11 :11] 
 ** Reserved           [10 :6 ] 
 ** BUSMON0_CHN_SEL    [5 :4 ] Busmon0 channel selection
 ** BUSMON0_CHN_MUX    [3]     Channel mux
 ** BUSMON0_CHN0_MUX   [2]     Channel mux
 ** SDIO_SLOT_SEL      [1 :0 ] SDIO slot selection
 ******************************* Field definition ******************************/
#define    BUSMON0_CHN2_MUX_SHIFT             (12)
#define    BUSMON0_CHN1_MUX_SHIFT             (11)
#define    BUSMON0_CHN_SEL_SHIFT                (4)
#define    BUSMON0_CHN_SEL_MSK                   (0x3 << BUSMON0_CHN_SEL_SHIFT)
#define    BUSMON0_CHN_MUX_SHIFT               (3)
#define    BUSMON0_CHN0_MUX_SHIFT             (2)
#define    SDIO_SLOT_SEL_SHIFT                      (0)
#define    SDIO_SLOT_SEL_MSK                        (0x3 << SDIO_SLOT_SEL_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL1
 ** Register Offset     : 0x00B4
 ** Description         : The MCU  system control register 1
 ** MTX_PRI_M3TOS5_RND_EN     [31:31] Matrix mater3 priority round-robin enable
 ** MTX_PRI_M3TOS5_ADJ_EN     [30:30] Matrix mater3 priority adjust enable
 ** MTX_PRI_M3TOS5_SEL        [29:28] Matrix mater3 priority level selection
 ** MTX_PRI_M3TOS5_RND_THR    [27:24] Matrix mater3 priority round-robin        
 **                                   threshold value
 ** MTX_PRI_M2TOS5_RND_EN     [23:23] Matrix mater2 priority round-robin enable
 ** MTX_PRI_M2TOS5_ADJ_EN     [22:22] Matrix mater2 priority adjust enable
 ** MTX_PRI_M2TOS5_SEL        [21:20] Matrix mater2 priority level selection
 ** MTX_PRI_M2TOS5_RND_THR    [19:16] Matrix mater2 priority round-robin        
 **                                   threshold value
 ** MTX_PRI_M1TOS5_RND_EN     [15:15] Matrix mater1 priority round-robin enable
 ** MTX_PRI_M1TOS5_ADJ_EN     [14:14] Matrix mater1 priority adjust enable
 ** MTX_PRI_M1TOS5_SEL        [13:12] Matrix mater1 priority level selection
 ** MTX_PRI_M1TOS5_RND_THR    [11:8 ] Matrix mater1 priority round-robin        
 **                                   threshold value
 ** MTX_PRI_M0TOS5_RND_EN     [7 :7 ] Matrix mater0 priority round-robin enable
 ** MTX_PRI_M0TOS5_ADJ_EN     [6 :6 ] Matrix mater0 priority adjust enable
 ** MTX_PRI_M0TOS5_SEL        [5 :4 ] Matrix mater0 priority level selection
 ** MTX_PRI_M0TOS5_RND_THR    [3 :0 ] Matrix mater0 priority round-robin        
 **                                   threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M3TOS5_RND_EN           (BIT_31)
#define    MTX_PRI_M3TOS5_ADJ_EN           (BIT_30)
#define    MTX_PRI_M3TOS5_SEL_SHIFT        (28)
#define    MTX_PRI_M3TOS5_SEL_MSK          (0x3 << MTX_PRI_M3TOS5_SEL_SHIFT)
#define    MTX_PRI_M3TOS5_RND_THR_SHIFT    (24)
#define    MTX_PRI_M3TOS5_RND_THR_MSK      (0xF << MTX_PRI_M3TOS5_RND_THR_SHIFT)
#define    MTX_PRI_M2TOS5_RND_EN           (BIT_23)
#define    MTX_PRI_M2TOS5_ADJ_EN           (BIT_22)
#define    MTX_PRI_M2TOS5_SEL_SHIFT        (20)
#define    MTX_PRI_M2TOS5_SEL_MSK          (0x3 << MTX_PRI_M2TOS5_SEL_SHIFT)
#define    MTX_PRI_M2TOS5_RND_THR_SHIFT    (16)
#define    MTX_PRI_M2TOS5_RND_THR_MSK      (0xF << MTX_PRI_M2TOS5_RND_THR_SHIFT)
#define    MTX_PRI_M1TOS5_RND_EN           (BIT_15)
#define    MTX_PRI_M1TOS5_ADJ_EN           (BIT_14)
#define    MTX_PRI_M1TOS5_SEL_SHIFT        (12)
#define    MTX_PRI_M1TOS5_SEL_MSK          (0x3 << MTX_PRI_M1TOS5_SEL_SHIFT)
#define    MTX_PRI_M1TOS5_RND_THR_SHIFT    (8)
#define    MTX_PRI_M1TOS5_RND_THR_MSK      (0xF << MTX_PRI_M1TOS5_RND_THR_SHIFT)
#define    MTX_PRI_M0TOS5_RND_EN           (BIT_7)
#define    MTX_PRI_M0TOS5_ADJ_EN           (BIT_6)
#define    MTX_PRI_M0TOS5_SEL_SHIFT        (4)
#define    MTX_PRI_M0TOS5_SEL_MSK          (0x3 << MTX_PRI_M0TOS5_SEL_SHIFT)
#define    MTX_PRI_M0TOS5_RND_THR_SHIFT    (0)
#define    MTX_PRI_M0TOS5_RND_THR_MSK      (0xF << MTX_PRI_M0TOS5_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL2
 ** Register Offset     : 0x00B8
 ** Description         : The MCU  system control register 2
 ** Reserved              [31:16]
 ** LCDC_BIGEND_EN        [15:15] LCDC AHB bigendian enable, active high
 ** ROTA_BIGEND_EN        [14:14] ROTA AHB bigendian enable, active high
 ** SHMEM_MAP_SWT         [13:13] SHMEM_MAP AHB bigendian enable, active high
 ** SHMEM_BIGEND_EN       [12:12] SHMEM AHB bigendian enable, active high
 ** LCM_ENDIAN_SEL        [11:11] LCDC AHB bigendian enable, active high
 ** Reserved              [10:6]
 ** ENDIAN_SEL_BUSMON0    [5 :5 ] Busmon0 bigendian enable, active high
 ** Reserved              [4 :4 ]
 ** ENDIAN_SEL_SDIO       [3 :3 ] SDIO bigendian enable, active high
 ** ENDIAN_SEL_DMA        [2 :2 ] DMA bigendian enable, active high
 ** Reserved              [1 :1 ]
 ** DCAM_BIGEND_EN        [0 :0 ] DMA bigendian enable, active high
 ******************************* Field definition ******************************/
#define    LCDC_BIGEND_EN              (BIT_15)
#define    ROTA_BIGEND_EN              (BIT_14)
#define    SHMEM_MAP_SWT               (BIT_13)
#define    SHMEM_BIGEND_EN             (BIT_12)
#define    LCM_ENDIAN_SEL              (BIT_11)
#define    ENDIAN_SEL_BUSMON0          (BIT_5)
#define    ENDIAN_SEL_SDIO             (BIT_3)
#define    ENDIAN_SEL_DMA              (BIT_2)
#define    DCAM_BIGEND_EN              (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL3
 ** Register Offset     : 0x00BC
 ** Description         : The MCU  system control register 3
 ** Reserved          [31:31]
 ** DMA_REQ_HW_SEL    [30:0 ] unused signal 
 ******************************* Field definition ******************************/
#define    DMA_REQ_HW_SEL_SHIFT    (0)
#define    DMA_REQ_HW_SEL_MSK      (0x7FFFFFFFUL << DMA_REQ_HW_SEL_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL4
 ** Register Offset     : 0x00C0
 ** Description         : The MCU  system control register 4
 ** MTX_PRI_M4TOS5_RND_EN     [31:31] Matrix mater4 priority round-robin enable
 ** MTX_PRI_M4TOS5_ADJ_EN     [30:30] Matrix mater4 priority adjust enable
 ** MTX_PRI_M4TOS5_SEL        [29:28] Matrix mater4 priority level selection
 ** MTX_PRI_M4TOS5_RND_THR    [27:24] Matrix mater4 priority round-robin        
 **                                   threshold value
 ** Reserved                                [23:8] 
 ** MTX_PRI_M10TOS5_RND_EN     [7 :7 ] Matrix mater10 priority round-robin enable
 ** MTX_PRI_M10TOS5_ADJ_EN     [6 :6 ] Matrix mater10 priority adjust enable
 ** MTX_PRI_M10TOS5_SEL        [5 :4 ] Matrix mater10 priority level selection
 ** MTX_PRI_M10TOS5_RND_THR    [3 :0 ] Matrix mater10 priority round-robin        
 **                                   threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M4TOS5_RND_EN           (BIT_31)
#define    MTX_PRI_M4TOS5_ADJ_EN           (BIT_30)
#define    MTX_PRI_M4TOS5_SEL_SHIFT        (28)
#define    MTX_PRI_M4TOS5_SEL_MSK          (0x3 << MTX_PRI_M4TOS5_SEL_SHIFT)
#define    MTX_PRI_M4TOS5_RND_THR_SHIFT    (24)
#define    MTX_PRI_M4TOS5_RND_THR_MSK      (0xF << MTX_PRI_M4TOS5_RND_THR_SHIFT)
#define    MTX_PRI_M10TOS5_RND_EN           (BIT_7)
#define    MTX_PRI_M10TOS5_ADJ_EN           (BIT_6)
#define    MTX_PRI_M10TOS5_SEL_SHIFT        (4)
#define    MTX_PRI_M10TOS5_SEL_MSK          (0x3 << MTX_PRI_M10TOS5_SEL_SHIFT)
#define    MTX_PRI_M10TOS5_RND_THR_SHIFT    (0)
#define    MTX_PRI_M10TOS5_RND_THR_MSK      (0xF << MTX_PRI_M10TOS5_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL5
 ** Register Offset     : 0x00C4
 ** Description         : The MCU  system control register 5
 ** MTX_PRI_M8TOS5_RND_EN     [31:31] Matrix mater8 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M8TOS5_ADJ_EN     [30:30] Matrix mater8 priority adjust enable
 ** MTX_PRI_M8TOS5_SEL        [29:28] Matrix mater8 priority level selection
 ** MTX_PRI_M8TOS5_RND_THR    [27:24] Matrix mater8 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M7TOS5_RND_EN     [23:23] Matrix mater7 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M7TOS5_ADJ_EN     [22:22] Matrix mater7 priority adjust enable
 ** MTX_PRI_M7TOS5_SEL        [21:20] Matrix mater7 priority level selection
 ** MTX_PRI_M7TOS5_RND_THR    [19:16] Matrix mater7 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M6TOS5_RND_EN      [15:15] Matrix mater6 priority round-robin enable
 ** MTX_PRI_M6TOS5_ADJ_EN      [14:14] Matrix mater6 priority adjust enable
 ** MTX_PRI_M6TOS5_SEL         [13:12] Matrix mater6 priority level selection
 ** MTX_PRI_M6TOS5_RND_THR     [11:8 ] Matrix mater6 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M5TOS5_RND_EN      [7 :7 ] Matrix mater5 priority round-robin enable
 ** MTX_PRI_M5TOS5_ADJ_EN      [6 :6 ] Matrix mater5 priority adjust enable
 ** MTX_PRI_M5TOS5_SEL         [5 :4 ] Matrix mater5 priority level selection
 ** MTX_PRI_M5TOS5_RND_THR     [3 :0 ] Matrix mater5 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M8TOS5_RND_EN           (BIT_31)
#define    MTX_PRI_M8TOS5_ADJ_EN           (BIT_30)
#define    MTX_PRI_M8TOS5_SEL_SHIFT        (28)
#define    MTX_PRI_M8TOS5_SEL_MSK          (0x3 << MTX_PRI_M8TOS5_SEL_SHIFT)
#define    MTX_PRI_M8TOS5_RND_THR_SHIFT    (24)
#define    MTX_PRI_M8TOS5_RND_THR_MSK      (0xF << MTX_PRI_M8TOS5_RND_THR_SHIFT)
#define    MTX_PRI_M7TOS5_RND_EN           (BIT_23)
#define    MTX_PRI_M7TOS5_ADJ_EN           (BIT_22)
#define    MTX_PRI_M7TOS5_SEL_SHIFT        (20)
#define    MTX_PRI_M7TOS5_SEL_MSK          (0x3 << MTX_PRI_M7TOS5_SEL_SHIFT)
#define    MTX_PRI_M7TOS5_RND_THR_SHIFT    (16)
#define    MTX_PRI_M7TOS5_RND_THR_MSK      (0xF << MTX_PRI_M7TOS5_RND_THR_SHIFT)
#define    MTX_PRI_M6TOS5_RND_EN            (BIT_15)
#define    MTX_PRI_M6TOS5_ADJ_EN            (BIT_14)
#define    MTX_PRI_M6TOS5_SEL_SHIFT     (12)
#define    MTX_PRI_M6TOS5_SEL_MSK       (0x3 << MTX_PRI_M6TOS5_SEL_SHIFT)
#define    MTX_PRI_M6TOS5_RND_THR_SHIFT     (8)
#define    MTX_PRI_M6TOS5_RND_THR_MSK       (0xF << MTX_PRI_M6TOS5_RND_THR_SHIFT)
#define    MTX_PRI_M5TOS5_RND_EN            (BIT_7)
#define    MTX_PRI_M5TOS5_ADJ_EN            (BIT_6)
#define    MTX_PRI_M5TOS5_SEL_SHIFT         (4)
#define    MTX_PRI_M5TOS5_SEL_MSK           (0x3 << MTX_PRI_M5TOS5_SEL_SHIFT)
#define    MTX_PRI_M5TOS5_RND_THR_SHIFT     (0)
#define    MTX_PRI_M5TOS5_RND_THR_MSK       (0xF << MTX_PRI_M5TOS5_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SLP_CTL0
 ** Register Offset     : 0x00D0
 ** Description         : The AHB sleep control register 0
 ** Reserved             [31:6 ]
 ** APB_STOP             [5 :5 ] Apb_stop Set this bit to close PCLK. Clear this
 **                               bit to open PCLK. 
 ** Reserved             [4 :2 ]
 ** MCU_CORE_SLP         [1 :1 ] Mcu_core_sleep Write 1 to this register will   
 **                              STOP the ARM CPU. ARM CPU will come out of STOP
 **                               state when either IRQ or FIQ interrupt or     
 **                              HRESETn is received.
 ** MCU_FORCE_SYS_SLP    [0 :0 ] Mcu_force_sys_sleep Write 1 to this register   
 **                              will STOP the ARM CPU and AHB. ARM CPU and AHB 
 **                              will come out of STOP state when either IRQ or 
 **                              FIQ interrupt or HRESETn is received. 
 ******************************* Field definition ******************************/
#define    APB_STOP                   (BIT_5)
#define    MCU_CORE_SLP               (BIT_1)
#define    MCU_FORCE_SYS_SLP          (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_SLP_CTL1
 ** Register Offset     : 0x00D4
 ** Description         : The AHB sleep control register 1
 ** Reserved              [31:9 ]
 ** STC_TX_SEL            [8 :8 ] Hold ARM signal selection when DSP is in TX   
 **                               mode: 1: select power on signal 0:select      
 **                               module on signal 
 ** STC_RX_SEL            [7 :7 ] Hold ARM signal selection when DSP is in TX   
 **                               mode: 1: select power on signal 0:select      
 **                               module on signal 
 ** DMA_SLP_MODE          [6 :6 ] unused signal 
 ** Reserved              [5 :5 ]
 ** MCU_DEEP_SLP_EN       [4 :4 ] Mcu_deep_sleep_en
 ** MCU_SYS_SLP_EN        [3 :3 ] Mcu_sys_sleep_en
 ** MCU_DMA_WAKE_UP_EN    [2 :2 ] Mcu_dma_wakeup_en
 ** Reserved              [1 :1 ]
 ** Reserved              [0 :0 ]
 ******************************* Field definition ******************************/
#define    STC_TX_SEL                  (BIT_8)
#define    STC_RX_SEL                  (BIT_7)
#define    DMA_SLP_MODE                (BIT_6)
#define    MCU_DEEP_SLP_EN             (BIT_4)
#define    MCU_SYS_SLP_EN              (BIT_3)
#define    MCU_DMA_WAKE_UP_EN          (BIT_2)

/*******************************************************************************
 ** Register Name       : AHB_SLP_CTL2
 ** Register Offset     : 0x00D8
 ** Description         : The AHB sleep control register 2
 ** Reserved           [31:31]
 ** DMA_REQ_WAKE_EN    [30:0 ] DMA request wakeup enable
 ******************************* Field definition ******************************/
#define    DMA_REQ_WAKE_EN_SHIFT    (0)
#define    DMA_REQ_WAKE_EN_MSK      (0x7FFFFFFFUL << DMA_REQ_WAKE_EN_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL6
 ** Register Offset     : 0x00E0
 ** Description         : The ARM system control register
 ** Reserved         [31:3 ]
 ** PTEST            [2 :2 ] PTEST flag
 ** ARM_BOOT_MODE    [1 :1 ] Arm boot mode, set by strappin pin 
 ** REMAP            [0 :0 ] Remap 0: address 0 mapped to external memory 1:    
 **                          address 0 mapped to on-chip ROM 
 ******************************* Field definition ******************************/
#define    PTEST                  (BIT_2)
#define    ARM_BOOT_MODE          (BIT_1)
#define    REMAP                  (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL7
 ** Register Offset     : 0x00E4
 ** Description         : The MCU  system control register 7
 ** Reserved                   [31:8]
 ** MTX_PRI_M9TOS5_RND_EN     [7 :7 ] Matrix mater9 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M9TOS5_ADJ_EN     [6 :6 ] Matrix mater9 priority adjust enable
 ** MTX_PRI_M9TOS5_SEL        [5 :4 ] Matrix mater9 priority level selection
 ** MTX_PRI_M9TOS5_RND_THR    [3 :0 ] Matrix mater9 priority round-robin      
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M9TOS5_RND_EN           (BIT_7)
#define    MTX_PRI_M9TOS5_ADJ_EN           (BIT_6)
#define    MTX_PRI_M9TOS5_SEL_SHIFT        (4)
#define    MTX_PRI_M9TOS5_SEL_MSK          (0x3 << MTX_PRI_M12TOS5_SEL_SHIFT)
#define    MTX_PRI_M9TOS5_RND_THR_SHIFT    (0)
#define    MTX_PRI_M9TOS5_RND_THR_MSK      (0xF << MTX_PRI_M12TOS5_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL8
 ** Register Offset     : 0x00E8
 ** Description         : The MCU  system control register 8
 ** MTX_PRI_M3TOS6_RND_EN     [31:31] Matrix mater3 priority round-robin enable
 ** MTX_PRI_M3TOS6_ADJ_EN     [30:30] Matrix mater3 priority adjust enable
 ** MTX_PRI_M3TOS6_SEL        [29:28] Matrix mater3 priority level selection
 ** MTX_PRI_M3TOS6_RND_THR    [27:24] Matrix mater3 priority round-robin        
 **                                   threshold value
 ** MTX_PRI_M2TOS6_RND_EN     [23:23] Matrix mater2 priority round-robin enable
 ** MTX_PRI_M2TOS6_ADJ_EN     [22:22] Matrix mater2 priority adjust enable
 ** MTX_PRI_M2TOS6_SEL        [21:20] Matrix mater2 priority level selection
 ** MTX_PRI_M2TOS6_RND_THR    [19:16] Matrix mater2 priority round-robin        
 **                                   threshold value
 ** MTX_PRI_M1TOS6_RND_EN     [15:15] Matrix mater1 priority round-robin enable
 ** MTX_PRI_M1TOS6_ADJ_EN     [14:14] Matrix mater1 priority adjust enable
 ** MTX_PRI_M1TOS6_SEL        [13:12] Matrix mater1 priority level selection
 ** MTX_PRI_M1TOS6_RND_THR    [11:8 ] Matrix mater1 priority round-robin        
 **                                   threshold value
 ** MTX_PRI_M0TOS6_RND_EN     [7 :7 ] Matrix mater0 priority round-robin enable
 ** MTX_PRI_M0TOS6_ADJ_EN     [6 :6 ] Matrix mater0 priority adjust enable
 ** MTX_PRI_M0TOS6_SEL        [5 :4 ] Matrix mater0 priority level selection
 ** MTX_PRI_M0TOS6_RND_THR    [3 :0 ] Matrix mater0 priority round-robin        
 **                                   threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M3TOS6_RND_EN           (BIT_31)
#define    MTX_PRI_M3TOS6_ADJ_EN           (BIT_30)
#define    MTX_PRI_M3TOS6_SEL_SHIFT        (28)
#define    MTX_PRI_M3TOS6_SEL_MSK          (0x3 << MTX_PRI_M3TOS6_SEL_SHIFT)
#define    MTX_PRI_M3TOS6_RND_THR_SHIFT    (24)
#define    MTX_PRI_M3TOS6_RND_THR_MSK      (0xF << MTX_PRI_M3TOS6_RND_THR_SHIFT)
#define    MTX_PRI_M2TOS6_RND_EN           (BIT_23)
#define    MTX_PRI_M2TOS6_ADJ_EN           (BIT_22)
#define    MTX_PRI_M2TOS6_SEL_SHIFT        (20)
#define    MTX_PRI_M2TOS6_SEL_MSK          (0x3 << MTX_PRI_M2TOS6_SEL_SHIFT)
#define    MTX_PRI_M2TOS6_RND_THR_SHIFT    (16)
#define    MTX_PRI_M2TOS6_RND_THR_MSK      (0xF << MTX_PRI_M2TOS6_RND_THR_SHIFT)
#define    MTX_PRI_M1TOS6_RND_EN           (BIT_15)
#define    MTX_PRI_M1TOS6_ADJ_EN           (BIT_14)
#define    MTX_PRI_M1TOS6_SEL_SHIFT        (12)
#define    MTX_PRI_M1TOS6_SEL_MSK          (0x3 << MTX_PRI_M1TOS6_SEL_SHIFT)
#define    MTX_PRI_M1TOS6_RND_THR_SHIFT    (8)
#define    MTX_PRI_M1TOS6_RND_THR_MSK      (0xF << MTX_PRI_M1TOS6_RND_THR_SHIFT)
#define    MTX_PRI_M0TOS6_RND_EN           (BIT_7)
#define    MTX_PRI_M0TOS6_ADJ_EN           (BIT_6)
#define    MTX_PRI_M0TOS6_SEL_SHIFT        (4)
#define    MTX_PRI_M0TOS6_SEL_MSK          (0x3 << MTX_PRI_M0TOS6_SEL_SHIFT)
#define    MTX_PRI_M0TOS6_RND_THR_SHIFT    (0)
#define    MTX_PRI_M0TOS6_RND_THR_MSK      (0xF << MTX_PRI_M0TOS6_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL9
 ** Register Offset     : 0x00EC
 ** Description         : The MCU  system control register 9
 ** MTX_PRI_M4TOS6_RND_EN     [31:31] Matrix mater7 priority round-robin enable
 ** MTX_PRI_M4TOS6_ADJ_EN     [30:30] Matrix mater7 priority adjust enable
 ** MTX_PRI_M4TOS6_SEL        [29:28] Matrix mater7 priority level selection
 ** MTX_PRI_M4TOS6_RND_THR    [27:24] Matrix mater7 priority round-robin        
 **                                   threshold value
 ** Reserved    [3 :0 ] 
 ******************************* Field definition ******************************/
#define    MTX_PRI_M4TOS6_RND_EN           (BIT_31)
#define    MTX_PRI_M4TOS6_ADJ_EN           (BIT_30)
#define    MTX_PRI_M4TOS6_SEL_SHIFT        (28)
#define    MTX_PRI_M4TOS6_SEL_MSK          (0x3 << MTX_PRI_M4TOS6_SEL_SHIFT)
#define    MTX_PRI_M4TOS6_RND_THR_SHIFT    (24)
#define    MTX_PRI_M4TOS6_RND_THR_MSK      (0xF << MTX_PRI_M4TOS6_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL10
 ** Register Offset     : 0x00F0
 ** Description         : The MCU  system control register 10
 ** MTX_PRI_M8TOS6_RND_EN     [31:31] Matrix mater8 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M8TOS6_ADJ_EN     [30:30] Matrix mater8 priority adjust enable
 ** MTX_PRI_M8TOS6_SEL        [29:28] Matrix mater8 priority level selection
 ** MTX_PRI_M8TOS6_RND_THR    [27:24] Matrix mater8 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M7TOS6_RND_EN     [23:23] Matrix mater7 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M7TOS6_ADJ_EN     [22:22] Matrix mater7 priority adjust enable
 ** MTX_PRI_M7TOS6_SEL        [21:20] Matrix mater7 priority level selection
 ** MTX_PRI_M7TOS6_RND_THR    [19:16] Matrix mater7 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M6TOS6_RND_EN      [15:15] Matrix mater6 priority round-robin enable
 ** MTX_PRI_M6TOS6_ADJ_EN      [14:14] Matrix mater6 priority adjust enable
 ** MTX_PRI_M6TOS6_SEL         [13:12] Matrix mater6 priority level selection
 ** MTX_PRI_M6TOS6_RND_THR     [11:8 ] Matrix mater6 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M5TOS6_RND_EN      [7 :7 ] Matrix mater5 priority round-robin enable
 ** MTX_PRI_M5TOS6_ADJ_EN      [6 :6 ] Matrix mater5 priority adjust enable
 ** MTX_PRI_M5TOS6_SEL         [5 :4 ] Matrix mater5 priority level selection
 ** MTX_PRI_M5TOS6_RND_THR     [3 :0 ] Matrix mater5 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M8TOS6_RND_EN           (BIT_31)
#define    MTX_PRI_M8TOS6_ADJ_EN           (BIT_30)
#define    MTX_PRI_M8TOS6_SEL_SHIFT        (28)
#define    MTX_PRI_M8TOS6_SEL_MSK          (0x3 << MTX_PRI_M8TOS6_SEL_SHIFT)
#define    MTX_PRI_M8TOS6_RND_THR_SHIFT    (24)
#define    MTX_PRI_M8TOS6_RND_THR_MSK      (0xF << MTX_PRI_M8TOS6_RND_THR_SHIFT)
#define    MTX_PRI_M7TOS6_RND_EN           (BIT_23)
#define    MTX_PRI_M7TOS6_ADJ_EN           (BIT_22)
#define    MTX_PRI_M7TOS6_SEL_SHIFT        (20)
#define    MTX_PRI_M7TOS6_SEL_MSK          (0x3 << MTX_PRI_M7TOS6_SEL_SHIFT)
#define    MTX_PRI_M7TOS6_RND_THR_SHIFT    (16)
#define    MTX_PRI_M7TOS6_RND_THR_MSK      (0xF << MTX_PRI_M7TOS6_RND_THR_SHIFT)
#define    MTX_PRI_M6TOS6_RND_EN            (BIT_15)
#define    MTX_PRI_M6TOS6_ADJ_EN            (BIT_14)
#define    MTX_PRI_M6TOS6_SEL_SHIFT         (12)
#define    MTX_PRI_M6TOS6_SEL_MSK           (0x3 << MTX_PRI_M6TOS6_SEL_SHIFT)
#define    MTX_PRI_M6TOS6_RND_THR_SHIFT     (8)
#define    MTX_PRI_M6TOS6_RND_THR_MSK       (0xF << MTX_PRI_M6TOS6_RND_THR_SHIFT)
#define    MTX_PRI_M5TOS6_RND_EN            (BIT_7)
#define    MTX_PRI_M5TOS6_ADJ_EN            (BIT_6)
#define    MTX_PRI_M5TOS6_SEL_SHIFT         (4)
#define    MTX_PRI_M5TOS6_SEL_MSK           (0x3 << MTX_PRI_M5TOS6_SEL_SHIFT)
#define    MTX_PRI_M5TOS6_RND_THR_SHIFT     (0)
#define    MTX_PRI_M5TOS6_RND_THR_MSK       (0xF << MTX_PRI_M5TOS6_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL11
 ** Register Offset     : 0x00F4
 ** Description         : The MCU  system control register 11
 ** Reserved                   [31:8]
 ** MTX_PRI_M9TOS6_RND_EN     [7 :7 ] Matrix mater9 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M9TOS6_ADJ_EN     [6 :6 ] Matrix mater9 priority adjust enable
 ** MTX_PRI_M9TOS6_SEL        [5 :4 ] Matrix mater9 priority level selection
 ** MTX_PRI_M9TOS6_RND_THR    [3 :0 ] Matrix mater9 priority round-robin      
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M9TOS6_RND_EN           (BIT_7)
#define    MTX_PRI_M9TOS6_ADJ_EN           (BIT_6)
#define    MTX_PRI_M9TOS6_SEL_SHIFT        (4)
#define    MTX_PRI_M9TOS6_SEL_MSK          (0x3 << MTX_PRI_M9TOS6_SEL_SHIFT)
#define    MTX_PRI_M9TOS6_RND_THR_SHIFT    (0)
#define    MTX_PRI_M9TOS6_RND_THR_MSK      (0xF << MTX_PRI_M9TOS6_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SLP_STS0
 ** Register Offset     : 0x0100
 ** Description         : The ARM sleep status 0
 ** Reserved        [31:20]
 ** LCM_STOP        [19:19] LCM stop status
 ** EMC_STOP        [18:18] EMC stop status
 ** SFC_STOP        [17:17] SFC stop status
 ** Reserved        [16:12]
 ** MTX_M11_STOP    [11:11] M11 stop status of matrix
 ** MTX_M10_STOP    [10:10] M10 stop status of matrix
 ** MTX_M9_STOP     [9 :9 ] M9 stop status of matrix
 ** MTX_M8_STOP     [8 :8 ] M8 stop status of matrix
 ** MTX_M7_STOP     [7 :7 ] M7 stop status of matrix
 ** MTX_M6_STOP     [6 :6 ] M6 stop status of matrix
 ** MTX_M5_STOP     [5 :5 ] M5 stop status of matrix
 ** MTX_M4_STOP     [4 :4 ] M4 stop status of matrix
 ** MTX_M3_STOP     [3 :3 ] M3 stop status of matrix
 ** MTX_M2_STOP     [2 :2 ] M2 stop status of matrix
 ** MTX_M1_STOP     [1 :1 ] M1 stop status of matrix
 ** MTX_M0_STOP     [0 :0 ] M0 stop status of matrix
 ******************************* Field definition ******************************/
#define    LCM_STOP              (BIT_19)
#define    EMC_STOP              (BIT_18)
#define    SFC_STOP              (BIT_17)
#define    MTX_M11_STOP          (BIT_11)
#define    MTX_M10_STOP          (BIT_10)
#define    MTX_M9_STOP           (BIT_9)
#define    MTX_M8_STOP           (BIT_8)
#define    MTX_M7_STOP           (BIT_7)
#define    MTX_M6_STOP           (BIT_6)
#define    MTX_M5_STOP           (BIT_5)
#define    MTX_M4_STOP           (BIT_4)
#define    MTX_M3_STOP           (BIT_3)
#define    MTX_M2_STOP           (BIT_2)
#define    MTX_M1_STOP           (BIT_1)
#define    MTX_M0_STOP           (BIT_0)

/*******************************************************************************
 ** Register Name       : AHB_SLP_DBG
 ** Register Offset     : 0x0104
 ** Description         : Chip deep sleep debug
 ** Reserved         [31:14]
 ** CHIP_SLP_STS         [13:8] chip sleep status.bit0:cm4 deep sleep;bit1:dsp peri sleep;bit2:apb_slp_en;bit3:clk_qbc_en;bit4:dsp deep sleep;bit5:mcu_deep_sleep.
 ** Reserved         [7:5]
 ** PLL_PD_STS    [4:0 ] pll status bit0:APLL clk en; bit1:cpll 26m en;bit2:cpll 64m fm en;bit3:cpll 64m bt en;bit4:cpll 832m en
 ******************************* Field definition ******************************/
#define    CHIP_SLP_STS_SHIFT    (8)
#define    CHIP_SLP_STS_MSK      (0x3FUL << CHIP_SLP_STS_SHIFT)
#define    PLL_PD_STS_SHIFT    (0)
#define    PLL_PD_STS_MSK      (0x1FUL << PLL_PD_STS_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_FUNC_TST_ADDR
 ** Register Offset     : 0x0108
 ** Description         : Function patten test address
 ** FUNC_TST_ADDR    [31:0 ] Function patten test address
 ******************************* Field definition ******************************/
#define    FUNC_TST_ADDR_SHIFT    (0)
#define    FUNC_TST_ADDR_MSK      (0xFFFFFFFFUL << FUNC_TST_ADDR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_ARCH_PORT
 ** Register Offset     : 0x0120
 ** Description         : ARCH_eb protection control register
 ** Reserved         [31:17]
 ** MCU_PROT         [16:16] MCU protect stattus
 ** ARCH_PROT_VAL    [15:0 ] Only when set 0xA1B2, ARCH_EB_CLR could be active. 
 ******************************* Field definition ******************************/
#define    MCU_PROT               (BIT_16)
#define    ARCH_PROT_VAL_SHIFT    (0)
#define    ARCH_PROT_VAL_MSK      (0xFFFFUL << ARCH_PROT_VAL_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL12
 ** Register Offset     : 0x0124
 ** Description         : The MCU  system control register 12
 ** MTX_PRI_M8TOS0_RND_EN     [31:31] Matrix mater8 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M8TOS0_ADJ_EN     [30:30] Matrix mater8 priority adjust enable
 ** MTX_PRI_M8TOS0_SEL        [29:28] Matrix mater8 priority level selection
 ** MTX_PRI_M8TOS0_RND_THR    [27:24] Matrix mater8 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M7TOS0_RND_EN     [23:23] Matrix mater7 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M7TOS0_ADJ_EN     [22:22] Matrix mater7 priority adjust enable
 ** MTX_PRI_M7TOS0_SEL        [21:20] Matrix mater7 priority level selection
 ** MTX_PRI_M7TOS0_RND_THR    [19:16] Matrix mater7 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M6TOS0_RND_EN      [15:15] Matrix mater6 priority round-robin enable
 ** MTX_PRI_M6TOS0_ADJ_EN      [14:14] Matrix mater6 priority adjust enable
 ** MTX_PRI_M6TOS0_SEL         [13:12] Matrix mater6 priority level selection
 ** MTX_PRI_M6TOS0_RND_THR     [11:8 ] Matrix mater6 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M1TOS0_RND_EN      [7 :7 ] Matrix mater1 priority round-robin enable
 ** MTX_PRI_M1TOS0_ADJ_EN      [6 :6 ] Matrix mater1 priority adjust enable
 ** MTX_PRI_M1TOS0_SEL         [5 :4 ] Matrix mater1 priority level selection
 ** MTX_PRI_M1TOS0_RND_THR     [3 :0 ] Matrix mater1 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M8TOS0_RND_EN           (BIT_31)
#define    MTX_PRI_M8TOS0_ADJ_EN           (BIT_30)
#define    MTX_PRI_M8TOS0_SEL_SHIFT        (28)
#define    MTX_PRI_M8TOS0_SEL_MSK          (0x3 << MTX_PRI_M8TOS0_SEL_SHIFT)
#define    MTX_PRI_M8TOS0_RND_THR_SHIFT    (24)
#define    MTX_PRI_M8TOS0_RND_THR_MSK      (0xF << MTX_PRI_M8TOS0_RND_THR_SHIFT)
#define    MTX_PRI_M7TOS0_RND_EN           (BIT_23)
#define    MTX_PRI_M7TOS0_ADJ_EN           (BIT_22)
#define    MTX_PRI_M7TOS0_SEL_SHIFT        (20)
#define    MTX_PRI_M7TOS0_SEL_MSK          (0x3 << MTX_PRI_M7TOS0_SEL_SHIFT)
#define    MTX_PRI_M7TOS0_RND_THR_SHIFT    (16)
#define    MTX_PRI_M7TOS0_RND_THR_MSK      (0xF << MTX_PRI_M7TOS0_RND_THR_SHIFT)
#define    MTX_PRI_M6TOS0_RND_EN            (BIT_15)
#define    MTX_PRI_M6TOS0_ADJ_EN            (BIT_14)
#define    MTX_PRI_M6TOS0_SEL_SHIFT         (12)
#define    MTX_PRI_M6TOS0_SEL_MSK           (0x3 << MTX_PRI_M6TOS0_SEL_SHIFT)
#define    MTX_PRI_M6TOS0_RND_THR_SHIFT     (8)
#define    MTX_PRI_M6TOS0_RND_THR_MSK       (0xF << MTX_PRI_M6TOS0_RND_THR_SHIFT)
#define    MTX_PRI_M1TOS0_RND_EN            (BIT_7)
#define    MTX_PRI_M1TOS0_ADJ_EN            (BIT_6)
#define    MTX_PRI_M1TOS0_SEL_SHIFT         (4)
#define    MTX_PRI_M1TOS0_SEL_MSK           (0x3 << MTX_PRI_M1TOS0_SEL_SHIFT)
#define    MTX_PRI_M1TOS0_RND_THR_SHIFT     (0)
#define    MTX_PRI_M1TOS0_RND_THR_MSK       (0xF << MTX_PRI_M1TOS0_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL13
 ** Register Offset     : 0x0128
 ** Description         : The MCU  system control register 13
 ** Reserved                    [31:8]
 ** MTX_PRI_M9TOS0_RND_EN      [7 :7 ] Matrix mater9 priority round-robin enable
 ** MTX_PRI_M9TOS0_ADJ_EN      [6 :6 ] Matrix mater9 priority adjust enable
 ** MTX_PRI_M9TOS0_SEL         [5 :4 ] Matrix mater9 priority level selection
 ** MTX_PRI_M9TOS0_RND_THR     [3 :0 ] Matrix mater9 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M9TOS0_RND_EN            (BIT_7)
#define    MTX_PRI_M9TOS0_ADJ_EN            (BIT_6)
#define    MTX_PRI_M9TOS0_SEL_SHIFT         (4)
#define    MTX_PRI_M9TOS0_SEL_MSK           (0x3 << MTX_PRI_M9TOS0_SEL_SHIFT)
#define    MTX_PRI_M9TOS0_RND_THR_SHIFT     (0)
#define    MTX_PRI_M9TOS0_RND_THR_MSK       (0xF << MTX_PRI_M9TOS0_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL14
 ** Register Offset     : 0x012C
 ** Description         : The MCU  system control register 14
 ** Reserved                    [31:8]
 ** MTX_PRI_M9TOS0_RND_EN      [7 :7 ] Matrix mater9 priority round-robin enable
 ** MTX_PRI_M9TOS0_ADJ_EN      [6 :6 ] Matrix mater9 priority adjust enable
 ** MTX_PRI_M9TOS0_SEL         [5 :4 ] Matrix mater9 priority level selection
 ** MTX_PRI_M9TOS0_RND_THR     [3 :0 ] Matrix mater9 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M9TOS1_RND_EN            (BIT_7)
#define    MTX_PRI_M9TOS1_ADJ_EN            (BIT_6)
#define    MTX_PRI_M9TOS1_SEL_SHIFT         (4)
#define    MTX_PRI_M9TOS1_SEL_MSK           (0x3 << MTX_PRI_M9TOS1_SEL_SHIFT)
#define    MTX_PRI_M9TOS1_RND_THR_SHIFT     (0)
#define    MTX_PRI_M9TOS1_RND_THR_MSK       (0xF << MTX_PRI_M9TOS1_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL15
 ** Register Offset     : 0x0130
 ** Description         : The MCU  system control register 15
 ** MTX_PRI_M8TOS0_RND_EN     [31:31] Matrix mater8 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M8TOS0_ADJ_EN     [30:30] Matrix mater8 priority adjust enable
 ** MTX_PRI_M8TOS0_SEL        [29:28] Matrix mater8 priority level selection
 ** MTX_PRI_M8TOS0_RND_THR    [27:24] Matrix mater8 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M7TOS0_RND_EN     [23:23] Matrix mater7 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M7TOS0_ADJ_EN     [22:22] Matrix mater7 priority adjust enable
 ** MTX_PRI_M7TOS0_SEL        [21:20] Matrix mater7 priority level selection
 ** MTX_PRI_M7TOS0_RND_THR    [19:16] Matrix mater7 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M6TOS0_RND_EN      [15:15] Matrix mater6 priority round-robin enable
 ** MTX_PRI_M6TOS0_ADJ_EN      [14:14] Matrix mater6 priority adjust enable
 ** MTX_PRI_M6TOS0_SEL         [13:12] Matrix mater6 priority level selection
 ** MTX_PRI_M6TOS0_RND_THR     [11:8 ] Matrix mater6 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M1TOS0_RND_EN      [7 :7 ] Matrix mater1 priority round-robin enable
 ** MTX_PRI_M1TOS0_ADJ_EN      [6 :6 ] Matrix mater1 priority adjust enable
 ** MTX_PRI_M1TOS0_SEL         [5 :4 ] Matrix mater1 priority level selection
 ** MTX_PRI_M1TOS0_RND_THR     [3 :0 ] Matrix mater1 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M8TOS1_RND_EN           (BIT_31)
#define    MTX_PRI_M8TOS1_ADJ_EN           (BIT_30)
#define    MTX_PRI_M8TOS1_SEL_SHIFT        (28)
#define    MTX_PRI_M8TOS1_SEL_MSK          (0x3 << MTX_PRI_M8TOS1_SEL_SHIFT)
#define    MTX_PRI_M8TOS1_RND_THR_SHIFT    (24)
#define    MTX_PRI_M8TOS1_RND_THR_MSK      (0xF << MTX_PRI_M8TOS1_RND_THR_SHIFT)
#define    MTX_PRI_M7TOS1_RND_EN           (BIT_23)
#define    MTX_PRI_M7TOS1_ADJ_EN           (BIT_22)
#define    MTX_PRI_M7TOS1_SEL_SHIFT        (20)
#define    MTX_PRI_M7TOS1_SEL_MSK          (0x3 << MTX_PRI_M7TOS1_SEL_SHIFT)
#define    MTX_PRI_M7TOS1_RND_THR_SHIFT    (16)
#define    MTX_PRI_M7TOS1_RND_THR_MSK      (0xF << MTX_PRI_M7TOS1_RND_THR_SHIFT)
#define    MTX_PRI_M6TOS1_RND_EN            (BIT_15)
#define    MTX_PRI_M6TOS1_ADJ_EN            (BIT_14)
#define    MTX_PRI_M6TOS1_SEL_SHIFT         (12)
#define    MTX_PRI_M6TOS1_SEL_MSK           (0x3 << MTX_PRI_M6TOS1_SEL_SHIFT)
#define    MTX_PRI_M6TOS1_RND_THR_SHIFT     (8)
#define    MTX_PRI_M6TOS1_RND_THR_MSK       (0xF << MTX_PRI_M6TOS1_RND_THR_SHIFT)
#define    MTX_PRI_M1TOS1_RND_EN            (BIT_7)
#define    MTX_PRI_M1TOS1_ADJ_EN            (BIT_6)
#define    MTX_PRI_M1TOS1_SEL_SHIFT         (4)
#define    MTX_PRI_M1TOS1_SEL_MSK           (0x3 << MTX_PRI_M1TOS1_SEL_SHIFT)
#define    MTX_PRI_M1TOS1_RND_THR_SHIFT     (0)
#define    MTX_PRI_M1TOS1_RND_THR_MSK       (0xF << MTX_PRI_M1TOS1_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL16
 ** Register Offset     : 0x0134
 ** Description         : The MCU  system control register 16
 ** Reserved                    [31:16]
 ** MTX_PRI_M9TOS2_RND_EN      [15:15] Matrix mater9 priority round-robin enable
 ** MTX_PRI_M9TOS2_ADJ_EN      [14:14] Matrix mater9 priority adjust enable
 ** MTX_PRI_M9TOS2_SEL         [13:12] Matrix mater9 priority level selection
 ** MTX_PRI_M9TOS2_RND_THR     [11:8 ] Matrix mater9 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M8TOS2_RND_EN      [7 :7 ] Matrix mater8 priority round-robin enable
 ** MTX_PRI_M8TOS2_ADJ_EN      [6 :6 ] Matrix mater8 priority adjust enable
 ** MTX_PRI_M8TOS2_SEL         [5 :4 ] Matrix mater8 priority level selection
 ** MTX_PRI_M8TOS2_RND_THR     [3 :0 ] Matrix mater8 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M9TOS2_RND_EN            (BIT_15)
#define    MTX_PRI_M9TOS2_ADJ_EN            (BIT_14)
#define    MTX_PRI_M9TOS2_SEL_SHIFT         (12)
#define    MTX_PRI_M9TOS2_SEL_MSK           (0x3 << MTX_PRI_M9TOS2_SEL_SHIFT)
#define    MTX_PRI_M9TOS2_RND_THR_SHIFT     (8)
#define    MTX_PRI_M9TOS2_RND_THR_MSK       (0xF << MTX_PRI_M9TOS2_RND_THR_SHIFT)
#define    MTX_PRI_M8TOS2_RND_EN            (BIT_7)
#define    MTX_PRI_M8TOS2_ADJ_EN            (BIT_6)
#define    MTX_PRI_M8TOS2_SEL_SHIFT         (4)
#define    MTX_PRI_M8TOS2_SEL_MSK           (0x3 << MTX_PRI_M8TOS2_SEL_SHIFT)
#define    MTX_PRI_M8TOS2_RND_THR_SHIFT     (0)
#define    MTX_PRI_M8TOS2_RND_THR_MSK       (0xF << MTX_PRI_M8TOS2_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL17
 ** Register Offset     : 0x0138
 ** Description         : The MCU  system control register 17
 ** MTX_PRI_M7TOS2_RND_EN     [31:31] Matrix mater7 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M7TOS2_ADJ_EN     [30:30] Matrix mater7 priority adjust enable
 ** MTX_PRI_M7TOS2_SEL        [29:28] Matrix mater7 priority level selection
 ** MTX_PRI_M7TOS2_RND_THR    [27:24] Matrix mater7 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M6TOS2_RND_EN      [23:23] Matrix mater6 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M6TOS2_ADJ_EN      [22:22] Matrix mater6 priority adjust enable
 ** MTX_PRI_M6TOS2_SEL         [21:20] Matrix mater6 priority level selection
 ** MTX_PRI_M6TOS2_RND_THR     [19:16] Matrix mater6 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M5TOS2_RND_EN      [15:15] Matrix mater5 priority round-robin enable
 ** MTX_PRI_M5TOS2_ADJ_EN      [14:14] Matrix mater5 priority adjust enable
 ** MTX_PRI_M5TOS2_SEL         [13:12] Matrix mater5 priority level selection
 ** MTX_PRI_M5TOS2_RND_THR     [11:8 ] Matrix mater5 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M4TOS2_RND_EN      [7 :7 ] Matrix mater4 priority round-robin enable
 ** MTX_PRI_M4TOS2_ADJ_EN      [6 :6 ] Matrix mater4 priority adjust enable
 ** MTX_PRI_M4TOS2_SEL         [5 :4 ] Matrix mater4 priority level selection
 ** MTX_PRI_M4TOS2_RND_THR     [3 :0 ] Matrix mater4 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M7TOS2_RND_EN           (BIT_31)
#define    MTX_PRI_M7TOS2_ADJ_EN           (BIT_30)
#define    MTX_PRI_M7TOS2_SEL_SHIFT        (28)
#define    MTX_PRI_M7TOS2_SEL_MSK          (0x3 << MTX_PRI_M7TOS2_SEL_SHIFT)
#define    MTX_PRI_M7TOS2_RND_THR_SHIFT    (24)
#define    MTX_PRI_M7TOS2_RND_THR_MSK      (0xF << MTX_PRI_M7TOS2_RND_THR_SHIFT)
#define    MTX_PRI_M6TOS2_RND_EN            (BIT_23)
#define    MTX_PRI_M6TOS2_ADJ_EN            (BIT_22)
#define    MTX_PRI_M6TOS2_SEL_SHIFT         (20)
#define    MTX_PRI_M6TOS2_SEL_MSK           (0x3 << MTX_PRI_M6TOS2_SEL_SHIFT)
#define    MTX_PRI_M6TOS2_RND_THR_SHIFT     (16)
#define    MTX_PRI_M6TOS2_RND_THR_MSK       (0xF << MTX_PRI_M6TOS2_RND_THR_SHIFT)
#define    MTX_PRI_M5TOS2_RND_EN            (BIT_15)
#define    MTX_PRI_M5TOS2_ADJ_EN            (BIT_14)
#define    MTX_PRI_M5TOS2_SEL_SHIFT         (12)
#define    MTX_PRI_M5TOS2_SEL_MSK           (0x3 << MTX_PRI_M5TOS2_SEL_SHIFT)
#define    MTX_PRI_M5TOS2_RND_THR_SHIFT     (8)
#define    MTX_PRI_M5TOS2_RND_THR_MSK       (0xF << MTX_PRI_M5TOS2_RND_THR_SHIFT)
#define    MTX_PRI_M4TOS2_RND_EN            (BIT_7)
#define    MTX_PRI_M4TOS2_ADJ_EN            (BIT_6)
#define    MTX_PRI_M4TOS2_SEL_SHIFT         (4)
#define    MTX_PRI_M4TOS2_SEL_MSK           (0x3 << MTX_PRI_M4TOS2_SEL_SHIFT)
#define    MTX_PRI_M4TOS2_RND_THR_SHIFT     (0)
#define    MTX_PRI_M4TOS2_RND_THR_MSK       (0xF << MTX_PRI_M4TOS2_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL18
 ** Register Offset     : 0x013C
 ** Description         : The MCU  system control register 18
 ** Reserved      [31:24] 
 ** MTX_PRI_M2TOS2_RND_EN      [23:23] Matrix mater2 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M2TOS2_ADJ_EN      [22:22] Matrix mater2 priority adjust enable
 ** MTX_PRI_M2TOS2_SEL         [21:20] Matrix mater2 priority level selection
 ** MTX_PRI_M2TOS2_RND_THR     [19:16] Matrix mater2 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M1TOS2_RND_EN      [15:15] Matrix mater1 priority round-robin enable
 ** MTX_PRI_M1TOS2_ADJ_EN      [14:14] Matrix mater1 priority adjust enable
 ** MTX_PRI_M1TOS2_SEL         [13:12] Matrix mater1 priority level selection
 ** MTX_PRI_M1TOS2_RND_THR     [11:8 ] Matrix mater1 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M0TOS2_RND_EN      [7 :7 ] Matrix mater0 priority round-robin enable
 ** MTX_PRI_M0TOS2_ADJ_EN      [6 :6 ] Matrix mater0 priority adjust enable
 ** MTX_PRI_M0TOS2_SEL         [5 :4 ] Matrix mater0 priority level selection
 ** MTX_PRI_M0TOS2_RND_THR     [3 :0 ] Matrix mater0 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M2TOS2_RND_EN            (BIT_23)
#define    MTX_PRI_M2TOS2_ADJ_EN            (BIT_22)
#define    MTX_PRI_M2TOS2_SEL_SHIFT         (20)
#define    MTX_PRI_M2TOS2_SEL_MSK           (0x3 << MTX_PRI_M2TOS2_SEL_SHIFT)
#define    MTX_PRI_M2TOS2_RND_THR_SHIFT     (16)
#define    MTX_PRI_M2TOS2_RND_THR_MSK       (0xF << MTX_PRI_M2TOS2_RND_THR_SHIFT)
#define    MTX_PRI_M1TOS2_RND_EN            (BIT_15)
#define    MTX_PRI_M1TOS2_ADJ_EN            (BIT_14)
#define    MTX_PRI_M1TOS2_SEL_SHIFT         (12)
#define    MTX_PRI_M1TOS2_SEL_MSK           (0x3 << MTX_PRI_M1TOS2_SEL_SHIFT)
#define    MTX_PRI_M1TOS2_RND_THR_SHIFT     (8)
#define    MTX_PRI_M1TOS2_RND_THR_MSK       (0xF << MTX_PRI_M1TOS2_RND_THR_SHIFT)
#define    MTX_PRI_M0TOS2_RND_EN            (BIT_7)
#define    MTX_PRI_M0TOS2_ADJ_EN            (BIT_6)
#define    MTX_PRI_M0TOS2_SEL_SHIFT         (4)
#define    MTX_PRI_M0TOS2_SEL_MSK           (0x3 << MTX_PRI_M0TOS2_SEL_SHIFT)
#define    MTX_PRI_M0TOS2_RND_THR_SHIFT     (0)
#define    MTX_PRI_M0TOS2_RND_THR_MSK       (0xF << MTX_PRI_M0TOS2_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL19
 ** Register Offset     : 0x0140
 ** Description         : The MCU  system control register 19
 ** MTX_PRI_M9TOS3_RND_EN      [31:31] Matrix mater9 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M9TOS3_ADJ_EN      [30:30] Matrix mater9 priority adjust enable
 ** MTX_PRI_M9TOS3_SEL         [29:28] Matrix mater9 priority level selection
 ** MTX_PRI_M9TOS3_RND_THR     [27:24] Matrix mater9 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M8TOS3_RND_EN      [23:23] Matrix mater8 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M8TOS3_ADJ_EN      [22:22] Matrix mater8 priority adjust enable
 ** MTX_PRI_M8TOS3_SEL         [21:20] Matrix mater8 priority level selection
 ** MTX_PRI_M8TOS3_RND_THR     [19:16] Matrix mater8 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M7TOS3_RND_EN      [15:15] Matrix mater7 priority round-robin enable
 ** MTX_PRI_M7TOS3_ADJ_EN      [14:14] Matrix mater7 priority adjust enable
 ** MTX_PRI_M7TOS3_SEL         [13:12] Matrix mater7 priority level selection
 ** MTX_PRI_M7TOS3_RND_THR     [11:8 ] Matrix mater7 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M6TOS3_RND_EN       [7 :7 ] Matrix mater6 priority round-robin enable
 ** MTX_PRI_M6TOS3_ADJ_EN       [6 :6 ] Matrix mater6 priority adjust enable
 ** MTX_PRI_M6TOS3_SEL          [5 :4 ] Matrix mater6 priority level selection
 ** MTX_PRI_M6TOS3_RND_THR      [3 :0 ] Matrix mater6 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M9TOS3_RND_EN            (BIT_31)
#define    MTX_PRI_M9TOS3_ADJ_EN            (BIT_30)
#define    MTX_PRI_M9TOS3_SEL_SHIFT         (28)
#define    MTX_PRI_M9TOS3_SEL_MSK           (0x3 << MTX_PRI_M9TOS3_SEL_SHIFT)
#define    MTX_PRI_M9TOS3_RND_THR_SHIFT     (24)
#define    MTX_PRI_M9TOS3_RND_THR_MSK       (0xF << MTX_PRI_M9TOS3_RND_THR_SHIFT)
#define    MTX_PRI_M8TOS3_RND_EN            (BIT_23)
#define    MTX_PRI_M8TOS3_ADJ_EN            (BIT_22)
#define    MTX_PRI_M8TOS3_SEL_SHIFT         (20)
#define    MTX_PRI_M8TOS3_SEL_MSK           (0x3 << MTX_PRI_M8TOS3_SEL_SHIFT)
#define    MTX_PRI_M8TOS3_RND_THR_SHIFT     (16)
#define    MTX_PRI_M8TOS3_RND_THR_MSK       (0xF << MTX_PRI_M8TOS3_RND_THR_SHIFT)
#define    MTX_PRI_M7TOS3_RND_EN            (BIT_15)
#define    MTX_PRI_M7TOS3_ADJ_EN            (BIT_14)
#define    MTX_PRI_M7TOS3_SEL_SHIFT         (12)
#define    MTX_PRI_M7TOS3_SEL_MSK           (0x3 << MTX_PRI_M7TOS3_SEL_SHIFT)
#define    MTX_PRI_M7TOS3_RND_THR_SHIFT     (8)
#define    MTX_PRI_M7TOS3_RND_THR_MSK       (0xF << MTX_PRI_M7TOS3_RND_THR_SHIFT)
#define    MTX_PRI_M6TOS3_RND_EN             (BIT_7)
#define    MTX_PRI_M6TOS3_ADJ_EN             (BIT_6)
#define    MTX_PRI_M6TOS3_SEL_SHIFT          (4)
#define    MTX_PRI_M6TOS3_SEL_MSK            (0x3 << MTX_PRI_M6TOS3_SEL_SHIFT)
#define    MTX_PRI_M6TOS3_RND_THR_SHIFT      (0)
#define    MTX_PRI_M6TOS3_RND_THR_MSK        (0xF << MTX_PRI_M6TOS3_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL20
 ** Register Offset     : 0x0144
 ** Description         : The MCU  system control register 20
 ** Reserved                    [31:8]
 ** MTX_PRI_M1TOS3_RND_EN       [7 :7 ] Matrix mater1 priority round-robin enable
 ** MTX_PRI_M1TOS3_ADJ_EN       [6 :6 ] Matrix mater1 priority adjust enable
 ** MTX_PRI_M1TOS3_SEL          [5 :4 ] Matrix mater1 priority level selection
 ** MTX_PRI_M1TOS3_RND_THR      [3 :0 ] Matrix mater1 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M1TOS3_RND_EN             (BIT_7)
#define    MTX_PRI_M1TOS3_ADJ_EN             (BIT_6)
#define    MTX_PRI_M1TOS3_SEL_SHIFT          (4)
#define    MTX_PRI_M1TOS3_SEL_MSK            (0x3 << MTX_PRI_M1TOS3_SEL_SHIFT)
#define    MTX_PRI_M1TOS3_RND_THR_SHIFT      (0)
#define    MTX_PRI_M1TOS3_RND_THR_MSK        (0xF << MTX_PRI_M1TOS3_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL21
 ** Register Offset     : 0x0148
 ** Description         : The MCU  system control register 21
 ** MTX_PRI_M9TOS4_RND_EN      [31:31] Matrix mater9 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M9TOS4_ADJ_EN      [30:30] Matrix mater9 priority adjust enable
 ** MTX_PRI_M9TOS4_SEL         [29:28] Matrix mater9 priority level selection
 ** MTX_PRI_M9TOS4_RND_THR     [27:24] Matrix mater9 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M8TOS4_RND_EN      [23:23] Matrix mater8 priority round-robin enabl
 **                                    e
 ** MTX_PRI_M8TOS4_ADJ_EN      [22:22] Matrix mater8 priority adjust enable
 ** MTX_PRI_M8TOS4_SEL         [21:20] Matrix mater8 priority level selection
 ** MTX_PRI_M8TOS4_RND_THR     [19:16] Matrix mater8 priority round-robin      
 **                                    threshold value
 ** MTX_PRI_M7TOS4_RND_EN      [15:15] Matrix mater7 priority round-robin enable
 ** MTX_PRI_M7TOS4_ADJ_EN      [14:14] Matrix mater7 priority adjust enable
 ** MTX_PRI_M7TOS4_SEL         [13:12] Matrix mater7 priority level selection
 ** MTX_PRI_M7TOS4_RND_THR     [11:8 ] Matrix mater7 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M6TOS4_RND_EN       [7 :7 ] Matrix mater6 priority round-robin enable
 ** MTX_PRI_M6TOS4_ADJ_EN       [6 :6 ] Matrix mater6 priority adjust enable
 ** MTX_PRI_M6TOS4_SEL          [5 :4 ] Matrix mater6 priority level selection
 ** MTX_PRI_M6TOS4_RND_THR      [3 :0 ] Matrix mater6 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M9TOS4_RND_EN            (BIT_31)
#define    MTX_PRI_M9TOS4_ADJ_EN            (BIT_30)
#define    MTX_PRI_M9TOS4_SEL_SHIFT         (28)
#define    MTX_PRI_M9TOS4_SEL_MSK           (0x3 << MTX_PRI_M9TOS4_SEL_SHIFT)
#define    MTX_PRI_M9TOS4_RND_THR_SHIFT     (24)
#define    MTX_PRI_M9TOS4_RND_THR_MSK       (0xF << MTX_PRI_M9TOS4_RND_THR_SHIFT)
#define    MTX_PRI_M8TOS4_RND_EN            (BIT_23)
#define    MTX_PRI_M8TOS4_ADJ_EN            (BIT_22)
#define    MTX_PRI_M8TOS4_SEL_SHIFT         (20)
#define    MTX_PRI_M8TOS4_SEL_MSK           (0x3 << MTX_PRI_M8TOS4_SEL_SHIFT)
#define    MTX_PRI_M8TOS4_RND_THR_SHIFT     (16)
#define    MTX_PRI_M8TOS4_RND_THR_MSK       (0xF << MTX_PRI_M8TOS4_RND_THR_SHIFT)
#define    MTX_PRI_M7TOS4_RND_EN            (BIT_15)
#define    MTX_PRI_M7TOS4_ADJ_EN            (BIT_14)
#define    MTX_PRI_M7TOS4_SEL_SHIFT         (12)
#define    MTX_PRI_M7TOS4_SEL_MSK           (0x3 << MTX_PRI_M7TOS4_SEL_SHIFT)
#define    MTX_PRI_M7TOS4_RND_THR_SHIFT     (8)
#define    MTX_PRI_M7TOS4_RND_THR_MSK       (0xF << MTX_PRI_M7TOS4_RND_THR_SHIFT)
#define    MTX_PRI_M6TOS4_RND_EN             (BIT_7)
#define    MTX_PRI_M6TOS4_ADJ_EN             (BIT_6)
#define    MTX_PRI_M6TOS4_SEL_SHIFT          (4)
#define    MTX_PRI_M6TOS4_SEL_MSK            (0x3 << MTX_PRI_M6TOS4_SEL_SHIFT)
#define    MTX_PRI_M6TOS4_RND_THR_SHIFT      (0)
#define    MTX_PRI_M6TOS4_RND_THR_MSK        (0xF << MTX_PRI_M6TOS4_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL22
 ** Register Offset     : 0x014C
 ** Description         : The MCU  system control register 22
 ** Reserved                    [31:16] 
 ** MTX_PRI_M11TOS4_RND_EN       [15:15] Matrix mater11 priority round-robin enable
 ** MTX_PRI_M11TOS4_ADJ_EN       [14:14] Matrix mater11 priority adjust enable
 ** MTX_PRI_M11TOS4_SEL          [13:12] Matrix mater11 priority level selection
 ** MTX_PRI_M11TOS4_RND_THR      [11:8 ] Matrix mater11 priority round-robin       
 **                                    threshold value
 ** MTX_PRI_M1TOS4_RND_EN       [7 :7 ] Matrix mater1 priority round-robin enable
 ** MTX_PRI_M1TOS4_ADJ_EN       [6 :6 ] Matrix mater1 priority adjust enable
 ** MTX_PRI_M1TOS4_SEL          [5 :4 ] Matrix mater1 priority level selection
 ** MTX_PRI_M1TOS4_RND_THR      [3 :0 ] Matrix mater1 priority round-robin       
 **                                    threshold value
 ******************************* Field definition ******************************/
#define    MTX_PRI_M11TOS4_RND_EN             (BIT_15)
#define    MTX_PRI_M11TOS4_ADJ_EN             (BIT_14)
#define    MTX_PRI_M11TOS4_SEL_SHIFT          (12)
#define    MTX_PRI_M11TOS4_SEL_MSK            (0x3 << MTX_PRI_M11TOS4_SEL_SHIFT)
#define    MTX_PRI_M11TOS4_RND_THR_SHIFT      (8)
#define    MTX_PRI_M11TOS4_RND_THR_MSK        (0xF << MTX_PRI_M11TOS4_RND_THR_SHIFT)
#define    MTX_PRI_M1TOS4_RND_EN             (BIT_7)
#define    MTX_PRI_M1TOS4_ADJ_EN             (BIT_6)
#define    MTX_PRI_M1TOS4_SEL_SHIFT          (4)
#define    MTX_PRI_M1TOS4_SEL_MSK            (0x3 << MTX_PRI_M1TOS4_SEL_SHIFT)
#define    MTX_PRI_M1TOS4_RND_THR_SHIFT      (0)
#define    MTX_PRI_M1TOS4_RND_THR_MSK        (0xF << MTX_PRI_M1TOS4_RND_THR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_SYS_CTL23
 ** Register Offset     : 0x0150
 ** Description         : The MCU  system control register 23
 ** Reserved                    [31:0] 
 ******************************* Field definition ******************************/

/*******************************************************************************
 ** Register Name       : AHB_DMA_SLP_CTL2
 ** Register Offset     : 0x0154
 ** Description         : The AHB sleep control register dma
 ** Reserved    [31: 0]
 ******************************* Field definition ******************************/

/*******************************************************************************
 ** Register Name       : AHB_DAP_AP_CTL
 ** Register Offset     : 0x0158
 ** Description         : DAP_AP_CTL
 ** Reserved            [31:3 ]
 ** AHB_DAP_FDMA_SEL      [2 :2 ] for  DAM / Function dma selection
 ** DAP_AB_S_VALID      [1 :1 ] for dap async bridge
 ** DAP_AB_BRIDGE_EN    [0 :0 ] for dap async bridge
 ******************************* Field definition ******************************/
#define AHB_DAP_FDMA_SEL                    BIT_2
#define AHB_DAP_AB_S_VALID                BIT_1
#define AHB_DAP_AB_BRIDGE_EN            BIT_0

/*******************************************************************************
 ** Register Name       : AHB_SENSOR_MODE
 ** Register Offset     : 0x0160
 ** Description         : SENSOR_MODE
 ** Reserved            [31:2 ]
 ** SENSOR_MODE      [1 :0 ] for mm sensor mode;2'b0:8bit mode.2'b1:sensor data1 is SDA1_0, 2'b2:sensor data1 is SDA1_1(refer to pin list)
 ******************************* Field definition ******************************/
#define AHB_SENSOR_MODE_BIT_SHIFT             (0)
#define AHB_SENSOR_MODE_BIT_MSK      (0x3UL << AHB_SENSOR_MODE_BIT_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_PTEST_BOOT_ADDR
 ** Register Offset     : 0x0160
 ** Description         : PTEST_BOOT_ADDR
 ** Reserved            [31:2 ]
 ** PTEST_BOOT_ADDR      [31 :0 ] ptest function test mode boot address
 ******************************* Field definition ******************************/
#define PTEST_BOOT_ADDR_SHIFT             (0)
#define PTEST_BOOT_ADDR_MSK      (0xFFFFFFFFUL << PTEST_BOOT_ADDR_SHIFT)

/*******************************************************************************
 ** Register Name       : AHB_CHIP_ID
 ** Register Offset     : 0x03FC
 ** Description         : Chip ID register
 ** ID_BASE    [31:8 ] Chip ID base
 ** MX_REG     [7 :0 ] Chip ID config
 ******************************* Field definition ******************************/
#define    ID_BASE_SHIFT    (8)
#define    ID_BASE_MSK      (0xFFFFFFUL << ID_BASE_SHIFT)
#define    MX_REG_SHIFT     (0)
#define    MX_REG_MSK       (0xFFUL << MX_REG_SHIFT)


/**----------------------------------------------------------------------------*
 **                      Compiler Flag                                         *
 **----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_SC6531EFM_REG_AHB_H_
