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
#ifndef _PIN_REG_SC6531EFM_H_
#define _PIN_REG_SC6531EFM_H_
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

//PIN_CTL_BASE  		0x8C000000
//ANA_PIN_CTL_BASE  0x82001100

#define PINMAP_REG_BASE     PIN_CTL_BASE
#define ANA_PINMAP_REG_BASE ANA_PIN_CTL_BASE
#define REG_ADDR(_x_)       (PINMAP_REG_BASE + _x_)
#define ANA_REG_ADDR(_x_)   (ANA_PINMAP_REG_BASE + _x_)

/*----------Digital Die Pin Control Register----------*/

#define PIN_CTRL_REG1                               REG_ADDR(0x0004)
#define PIN_RFCTL0_REG        	                REG_ADDR(0x0020)        
#define PIN_RFCTL1_REG         	                REG_ADDR(0x0024)       
#define PIN_RF_PAEN_REG        	                REG_ADDR(0x0028)        
#define PIN_KEYIN0_REG        	                REG_ADDR(0x002C)        
#define PIN_KEYIN1_REG         	                REG_ADDR(0x0030)       
#define PIN_KEYIN2_REG        	                REG_ADDR(0x0034)        
#define PIN_KEYIN3_REG       		        REG_ADDR(0x0038)         
#define PIN_KEYIN4_REG        	                REG_ADDR(0x003C)        
#define PIN_KEYOUT0_REG                         REG_ADDR(0x0040)       
#define PIN_KEYOUT1_REG                         REG_ADDR(0x0044)       
#define PIN_KEYOUT2_REG                         REG_ADDR(0x0048)       
#define PIN_KEYOUT3_REG                         REG_ADDR(0x004C)       
#define PIN_KEYOUT4_REG       	                REG_ADDR(0x0050)
#define PIN_EXT_RST_B_REG			REG_ADDR(0x0054)
#define PIN_CHIP_SLEEP_REG			REG_ADDR(0x0058)
#define PIN_AUD_SCLK_REG			REG_ADDR(0x005C)
#define PIN_AUD_DASYNC_REG			REG_ADDR(0x0060)
#define PIN_AUD_DAD0_REG			REG_ADDR(0x0064)
#define PIN_AUD_DAD1_REG			REG_ADDR(0x0068)
#define PIN_AUD_ADSYNC_REG		        REG_ADDR(0x006C)
#define PIN_AUD_ADD0_REG			REG_ADDR(0x0070)
#define PIN_XTL_BUF_EN_REG			REG_ADDR(0x0074)
#define PIN_CLK_32K_REG				REG_ADDR(0x0078)
#define PIN_ANA_INT_REG				REG_ADDR(0x007C)
#define PIN_PTEST_REG				REG_ADDR(0x0080)
#define PIN_ADI_SCLK_REG			REG_ADDR(0x0084)
#define PIN_ADI_D_REG  				REG_ADDR(0x0088)       
#define PIN_IISCLK_REG        	                REG_ADDR(0x008C)   
#define PIN_IISDI_REG        		        REG_ADDR(0x0090)        
#define PIN_IISLRCK_REG        	                REG_ADDR(0x0094)        
#define PIN_IISDO_REG        		        REG_ADDR(0x0098)        
#define PIN_SD_D0_REG        		        REG_ADDR(0x009C)        
#define PIN_SD_D1_REG        		        REG_ADDR(0x00A0)        
#define PIN_SD_D2_REG       		        REG_ADDR(0x00A4)         
#define PIN_SD_D3_REG        		        REG_ADDR(0x00A8)        
#define PIN_SD_CMD_REG         	                REG_ADDR(0x00AC)       
#define PIN_SD_CLK_REG         	                REG_ADDR(0x00B0)       
#define PIN_EMDQS0_REG         	                REG_ADDR(0x00B4)       
#define PIN_EMD7_REG         		        REG_ADDR(0x00B8)       
#define PIN_EMD6_REG        		        REG_ADDR(0x00BC)        
#define PIN_EMD5_REG        		        REG_ADDR(0x00C0)        
#define PIN_EMD4_REG         		        REG_ADDR(0x00C4)       
#define PIN_EMCLK_REG         	                REG_ADDR(0x00C8)       
#define PIN_EMCLKN_REG         	                REG_ADDR(0x00CC)       
#define PIN_EMCEN1_REG         	                REG_ADDR(0x00D0)       
#define PIN_EMD3_REG         		        REG_ADDR(0x00D4)       
#define PIN_EMD2_REG         		        REG_ADDR(0x00D8)       
#define PIN_EMD1_REG         		        REG_ADDR(0x00DC)       
#define PIN_EMD0_REG         		        REG_ADDR(0x00E0)       
#define PIN_EMLBN_REG         	                REG_ADDR(0x00E4)       
#define PIN_SIMRST1_REG        	                REG_ADDR(0x00E8)        
#define PIN_SIMCLK1_REG                          REG_ADDR(0x00EC)       
#define PIN_SIMDA1_REG       		        REG_ADDR(0x00F0)         
#define PIN_SIMDA0_REG       		        REG_ADDR(0x00F4)         
#define PIN_SIMCLK0_REG        	                REG_ADDR(0x00F8)        
#define PIN_SIMRST0_REG        	                REG_ADDR(0x00FC)        
#define PIN_SPI0_CLK_REG                        REG_ADDR(0x0100)     
#define PIN_SPI0_DI_REG                           REG_ADDR(0x0104)     
#define PIN_SPI0_CS0_REG                        REG_ADDR(0x0108)     
#define PIN_SPI0_DO_REG                         REG_ADDR(0x010C)
#define PIN_CLK_AUX_REG                         REG_ADDR(0x0110)     
#define PIN_ESMD0_REG        		        REG_ADDR(0x0114)        
#define PIN_ESMD2_REG          	                REG_ADDR(0x0118)      
#define PIN_ESMD1_REG          	                REG_ADDR(0x011C)      
#define PIN_ESMCLK_REG                            REG_ADDR(0x0120)      
#define PIN_ESMD3_REG          	                REG_ADDR(0x0124)      
#define PIN_ESMSMP_REG                           REG_ADDR(0x0128)      
#define PIN_ESMCSN0_REG                         REG_ADDR(0x012C)      
#define PIN_EXTINT0_REG       	                REG_ADDR(0x0130)         
#define PIN_EXTINT1_REG       	                REG_ADDR(0x0134)         
#define PIN_SPI1_DI_REG                           REG_ADDR(0x0138)       
#define PIN_SPI1_CLK_REG                        REG_ADDR(0x013C)       
#define PIN_SPI1_DO_REG                         REG_ADDR(0x0140)       
#define PIN_SPI1_CS_REG                         REG_ADDR(0x0144)       
#define PIN_SPI1_CD_REG                         REG_ADDR(0x0148)       
#define PIN_FMARK_REG         	               REG_ADDR(0x014C)       
#define PIN_LCMRSTN_REG                        REG_ADDR(0x0150)           
#define PIN_SCL0_REG         		       REG_ADDR(0x0154)       
#define PIN_SDA0_REG         		       REG_ADDR(0x0158)       
#define PIN_CMPCLK_REG        	               REG_ADDR(0x015C)        
#define PIN_CCIRMCK_REG        	               REG_ADDR(0x0160)
#define PIN_CCIRVS_REG			       REG_ADDR(0x0164)
#define PIN_CAM_RSTN_REG                      REG_ADDR(0x0168)
#define PIN_CAM_PD_REG        	               REG_ADDR(0x016C)
#define PIN_CCIRD0_REG        	               REG_ADDR(0x0170)        
#define PIN_CCIRD2_REG        	               REG_ADDR(0x0174)        
#define PIN_CCIRD1_REG        	               REG_ADDR(0x0178)        
#define PIN_CCIRD3_REG        	               REG_ADDR(0x017C)        
#define PIN_CCIRD4_REG        	               REG_ADDR(0x0180)        
#define PIN_CCIRD5_REG        	               REG_ADDR(0x0184)        
#define PIN_CCIRD6_REG        	               REG_ADDR(0x0188)        
#define PIN_CCIRD7_REG      		       REG_ADDR(0x018C)          
#define PIN_CCIRHS_REG                           REG_ADDR(0x0190)
#define PIN_U0TXD_REG          	               REG_ADDR(0x0194)      
#define PIN_PWM_REG          		       REG_ADDR(0x0198)
#define PIN_U0RXD_REG          	               REG_ADDR(0x019C)
#define PIN_U0CTS_REG                             REG_ADDR(0x01A0)  
#define PIN_U0RTS_REG          	               REG_ADDR(0x01A4)      
#define PIN_MTCK_REG          	               REG_ADDR(0x01A8)      
#define PIN_MTMS_REG           	               REG_ADDR(0x01AC)     
#define PIN_GPIO_0_REG                           REG_ADDR(0x01B0)     
#define PIN_GPIO_1_REG                           REG_ADDR(0x01B4)     
#define PIN_GPIO_3_REG                           REG_ADDR(0x01B8)     
#define PIN_GPIO_2_REG                           REG_ADDR(0x01BC)

/*----------Analog Die Pin Control Register----------*/
#define   ANA_PIN_PTEST_REG                     ANA_REG_ADDR(0x0000)  
#define   ANA_PIN_ADI_SCLK_REG               ANA_REG_ADDR(0x0004)  
#define   ANA_PIN_ADI_D_REG                     ANA_REG_ADDR(0x0008)  
#define   ANA_PIN_XTL_BUF_EN_REG          ANA_REG_ADDR(0x000C)  
#define   ANA_PIN_CHIP_SLEEP_REG           ANA_REG_ADDR(0x0010)  
#define   ANA_PIN_CLK_32K_REG                 ANA_REG_ADDR(0x0014)  
#define   ANA_PIN_ANA_INT_REG                 ANA_REG_ADDR(0x0018)  
#define   ANA_PIN_AUD_ADSYNC_REG         ANA_REG_ADDR(0x001C)  
#define   ANA_PIN_AUD_ADD0_REG              ANA_REG_ADDR(0x0020)  
#define   ANA_PIN_AUD_DASYNC_REG         ANA_REG_ADDR(0x0024)  
#define   ANA_PIN_AUD_DAD0_REG              ANA_REG_ADDR(0x0028)  
#define   ANA_PIN_AUD_DAD1_REG              ANA_REG_ADDR(0x002C)  
#define   ANA_PIN_AUD_SCLK_REG               ANA_REG_ADDR(0x0030)  
#define   ANA_PIN_EXT_RST_B_REG              ANA_REG_ADDR(0x0034)  

/* Pinmap ctrl register Bit field value
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|                              |                		  |			 |			    |                      |                     |                     |                        |            |            |		      |		      |
| Reserved[31:22]   | Drv str sel[21:19]   | Dslp en[13]  | Bsr wpus[12]   | Bse se[11]    | func PU[7]    | func PD[6]   | func sel[5:4]    | PU[3]  | PD[2]  | input En[1] | output En[0] |
|                              |                                |		         |      		    |                      |                     |                     |                        |             |	        |		       |		      |
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
#define PIN_Z_EN                                 0x00            // High-Z in sleep mode
#define PIN_O_EN                                BIT_0           // Output enable in sleep mode
#define PIN_I_EN                                 BIT_1           // Input enable in sleep mode

#define PIN_SLP_Z				0x00				//High-Z in sleep mode
#define PIN_SLP_OE				BIT_0				//Output enable in sleep mode
#define PIN_SLP_IE				BIT_1				//Input enable in sleeep mode

#define PIN_SPD_EN				BIT_2			// Pull down enable for sleep mode
#define PIN_SPU_EN				BIT_3			// Pull up enable for sleep mode
#define PIN_SPX_EN				0x00			// Don't pull down or up

#define PIN_FUNC_DEF                        (0x00<<4)	
#define PIN_FUNC_0		  	        (0x00<<4)		    //Function select,BIT4-5
#define PIN_FUNC_1                             (0x01<<4)
#define PIN_FUNC_2                            (0x02<<4)
#define PIN_FUNC_3                            (0x03<<4)

#define PIN_FPD_EN				BIT_6			// Weak pull down for function mode
#define PIN_FPU_EN				BIT_7			// Weak pull up for function mode
#define PIN_FPX_EN				0x00			// Don't pull down or up

#define PIN_BSR_EN				BIT_11		//schmitt trigger input enable,active high
#define PIN_BSR_WPUS			BIT_12	    //4.7K pull-up resistor  
#define PIN_BSR_WPX                         0x00        //60K pull-up resistor

#define PIN_DSLP_EN				BIT_13
#define PIN_DSLP_X				0x00

#define PIN_DS_0					(0x00<<19)			// Driver strength level 0 BIT19-20
#define PIN_DS_1					(0x01<<19)			// Driver strength level 1 BIT19-20
#define PIN_DS_2					(0x02<<19)			// Driver strength level 2 BIT19-20
#define PIN_DS_3					(0x03<<19)			// Driver strength level 3 BIT19-20
#define PIN_DS_4                                (0x04<<19)          // 3V drv
#define PIN_DS_5                                (0x05<<19)          // 3V drv
#define PIN_DS_6                                (0x06<<19)          // 3V drv
#define PIN_DS_7                                (0x07<<19)          // 3V drv

/* Pinmap control register bit field value structure*/
typedef union _pinmap_ctl_reg_u
{
    volatile int dwValue;
    struct pinmap_ctl_reg_tag
    {
        volatile int wpd_campd               	:1;
        volatile int wpd_ddsdpd             	:1;
        volatile int wpd_ddio28pd0        	:1;
        volatile int wpd_ddioadpd      		:1;
        volatile int wpd_lcmpd               	:1;
        volatile int wpd_mempd             	:1;
        volatile int wpd_lcampd           	:1;
        volatile int wpd_sim0pd                  :1;
        volatile int wpd_sim1pd             	:1;
        volatile int wpd_memspd               	:1;
        volatile int wpd_ddio28pd1            :1;
        volatile int wpd_ddio28pd2            :1;
        volatile int wpd_ddio28pd3            :1;
        volatile int wpd_ddio28pd4            :1;
    } mBits;
} PINMAP_CTL_REG_U;

/* Adie Pinmap ctrl register Bit field value
--------------------------------------------------------------------------------------------------------------------------
|                             |                         |                   |                  |                       |            |           |                    |                       |
| Reserved[31:10] | Drv str sel[9:8] | func PU[7] | func PD[6] | func sel[5:4]  | PU[3]   | PD[2] | input En[1] | output En[0]   |
|                             |                         |                   |                  |                       |            |           |                    |                       |
--------------------------------------------------------------------------------------------------------------------------
*/

#define ANA_PIN_Z_EN                      0x00            // High-Z in sleep mode
#define ANA_PIN_O_EN                     BIT_0           // Output enable in sleep mode
#define ANA_PIN_I_EN                      BIT_1           // Input enable in sleep mode

#define ANA_PIN_SPD_EN                   BIT_2           // Pull down enable
#define ANA_PIN_SPU_EN                   BIT_3           // Pull up enable
#define ANA_PIN_SPX_EN                   0x00            // Don't pull down or up

#define ANA_PIN_FUNC_DEF            (0x00<<4)       //Function select,BIT4-5
#define ANA_PIN_FUNC_1                (0x01<<4)
#define ANA_PIN_FUNC_2                (0x02<<4)
#define ANA_PIN_FUNC_3                (0x03<<4)

#define ANA_PIN_FPD_EN                BIT_6           // Weak pull down for function mode
#define ANA_PIN_FPU_EN                BIT_7           // Weak pull up for function mode
#define ANA_PIN_FPX_EN                0x00            // Don't pull down or up

#define ANA_PIN_DS_0                    (0x00<<8)           // Driver strength level 0 BIT8-9
#define ANA_PIN_DS_1                    (0x01<<8)           // Driver strength level 1 BIT8-9
#define ANA_PIN_DS_2                    (0x02<<8)           // Driver strength level 2 BIT8-9
#define ANA_PIN_DS_3                    (0x03<<8)           // Driver strength level 3 BIT8-9

#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
