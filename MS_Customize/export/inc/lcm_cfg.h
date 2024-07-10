/******************************************************************************
 ** File Name:      lcm_cfg.h                                                 *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           01/15/2008                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of LCM   *
 **					about product
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/15/2008     Liangwen.Zhen    Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

#ifndef _LCM_CFG_H_
#define _LCM_CFG_H_

#include "lcd_backlight.h"
#include "dal_lcd.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif



#define TWO_DATA_LINE_LCD 1
#define ONE_DATA_LINE_LCD 0
#define USE_3_LINE_LCD 3  // this mode can't support two data line mode .



#define ENABLE_FMARK_LCD 1 // now ,only support two data line mode lcd.
#define DISABLE_FMARK_LCD  0
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
#define LCD_DRV_ID_ILI9225                      0x9225
#define LCD_DRV_ID_ILI9320                  	0x9320
#define LCD_DRV_ID_ILI9325                  	0x9325
#define LCD_DRV_ID_ILI9326                  	0x9326
#define LCD_DRV_ID_ILI9328                      0x9328
#define LCD_DRV_ID_ILI9335                  	0x9335
#define LCD_DRV_ID_HX8347                   	0x0047
#define LCD_DRV_ID_HX8347D						0x0047
#define LCD_DRV_ID_HX8357                   	0x0057
#define LCD_DRV_ID_R61505U                  	0x1505
#define LCD_DRV_ID_NT3911                   	0x3911
#define LCD_DRV_ID_NT39118                  	0x0023
#define LCD_DRV_ID_SPFD54124A           		0x004F
#define LCD_DRV_ID_SPFD5408           			0x5408
#define LCD_DRV_ID_SPFD5420           			0x5420
#define LCD_DRV_ID_LGDP4531           			0x4531
#define LCD_DRV_ID_LGDP4532           			0x4532
#define LCD_DRV_ID_R61509V                  	0xB509
#define LCD_DRV_ID_S6D04H0A                     0xB510
#define LCD_DRV_ID_ILI9342		                0x9342
#define LCD_DRV_ID_RM68040		                0x68040
#define LCD_DRV_ID_ILI9481		                0x9481
#define LCD_DRV_ID_ILI9486		                0x9486
#define LCD_DRV_ID_BYD8817F		                0x8817
#define LCD_DRV_ID_HX8340B                     0x0049
#define LCD_DRV_ID_HX8347G						0x0075
#define LCD_DRV_ID_ILI9338B                  	0x9338
#define LCD_DRV_ID_HX8352C                      0x0072
#define LCD_DRV_ID_R61529                       0x1529    // TODO: TBD

#define LCD_DRV_ID_S6D0139						0xFFFF
#define LCD_DRV_ID_ST7789H2                     0x8552
#define LCD_DRV_ID_GC9304C                      0x9304
#define LCD_DRV_ID_GC9305                      0x009305
#define LCD_DRV_ID_GC9306                      0x009306
#define LCD_DRV_ID_GC9a01                      0x009a01






#define LCD_DRV_ID_GC9307                      0x009307    
#define LCD_DRV_ID_JD9851                      0x1c90f5   //0x9290f5
#define LCD_DRV_ID_JD9851_2                    0x9290f5
#define LCD_DRV_ID_JD9851_3                    0x99ffff

#define LCD_DRV_ID_ST7789V2                    0x858552
#define LCD_DRV_ID_ST7789V3                    0x1c81F5
#define LCD_DRV_ID_ST7789V4                    0x3e80F5
#define LCD_DRV_ID_ST7789V5                    0x1c80F5

#define LCD_DRV_ID_ST7789P3                     0x8181B3 // FIHNJ // #if defined(DRV_LCD_7789P3_240X284) W217_W18_DRV_LCD_7789P3_240X284

/**---------------------------------------------------------------------------*
 **                      DATA STRUCTURE Definition                            *
 **---------------------------------------------------------------------------*/

 // define in tp configure layer
typedef struct
{
	uint32 reserved;

}TP_CFG_INFO_T, *TP_CFG_INFO_T_PTR;


typedef struct
{
	uint32						dev_id;			// ID of lcd driver product ic
	uint32						adc_chn;		//
	uint16						adc_min;		// 1 use two data line mode .
	uint16						adc_max;		// 1 enable lcd fmark.(only support two data line mode)
	LCD_SPEC_INFO_T         	spec_info;	    // Pointer of LCD control information
	LCD_BACKLIGHT_INFO_T        bl_control;     // back light information
	TP_CFG_INFO_T_PTR			tp_info_ptr;	// Pointer of TP module information enbed in LCM
         char              name[16];
}LCM_CFG_INFO_T,*LCM_CFG_INFO_T_PTR;

/**---------------------------------------------------------------------------*
 **                     Pblic Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get LCM configure information
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
PUBLIC LCM_CFG_INFO_T_PTR LCM_GetCfgInfo(uint32 lcd_id, uint32* count);


/******************************************************************************/
//  Description:  return the ILI9320 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9320_GetOperations(void);

PUBLIC BOOLEAN ILI9320_Probe(void);



/******************************************************************************/
//  Description:  return the ILI9325 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9325_GetOperations(void);
PUBLIC BOOLEAN ILI9325_Probe(void);

/******************************************************************************/
//  Description:  return the ILI9325B8 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9325B8_GetOperations(void);
PUBLIC BOOLEAN ILI9325B8_Probe(void);

/******************************************************************************/
//  Description:  return the ILI9326 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9326_GetOperations(void);
PUBLIC BOOLEAN ILI9326_Probe(void);

/******************************************************************************/
//  Description:  return the ILI9328 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9328_GetOperations(void);
PUBLIC BOOLEAN ILI9328_Probe(void);



/******************************************************************************/
//  Description:  return the ILI9335 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* ILI9335_GetOperations(void);
PUBLIC BOOLEAN ILI9335_Probe(void);


/******************************************************************************/
//  Description:  return the LGDP4531 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* LGDP4531_GetOperations(void);
PUBLIC BOOLEAN LGDP4531_Probe(void);

/******************************************************************************/
//  Description:  return the LGDP4532 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* LGDP4532_GetOperations(void);
PUBLIC BOOLEAN LGDP4532_Probe(void);

/**************************************************************************************/
// Description: get S1D19105 driver function
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8312_GetOperations(void);
PUBLIC BOOLEAN HX8312_Probe(void);

/******************************************************************************/
//  Description:  return the HX8346 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8346_GetOperations(void);
PUBLIC BOOLEAN HX8346_Probe(void);

/******************************************************************************/
//  Description:  return the HX8347 lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* HX8347_GetOperations(void);
PUBLIC BOOLEAN HX8347_Probe(void);

/**************************************************************************************/
// Description: get S1D19105 driver function
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* S6D0139_GetOperations(void);
PUBLIC BOOLEAN S6D0139_Probe(void);


/******************************************************************************/
//  Description:  return the SPFD54124A lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* SPFD54124A_GetOperations();
PUBLIC BOOLEAN SPFD54124A_Probe(void);

/******************************************************************************/
//  Description:  return the SPFD5420A lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* SPFD5420A_GetOperations(void);
PUBLIC BOOLEAN SPFD5420A_Probe(void);


/******************************************************************************/
//  Description:  return the R61505U lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* R61505U_GetOperations();
PUBLIC BOOLEAN R61505U_Probe(void);


/******************************************************************************/
//  Description:  return the R61509V lcd driver funtion pointer
//	Global resource dependence:
//  Author:         Jim.zhang
//	Note:
/******************************************************************************/
PUBLIC LCD_OPERATIONS_T* R61509V_GetOperations(void);
PUBLIC BOOLEAN R61509V_Probe(void);


/**************************************************************************************/
// Description: get S1D19105 driver function
// Global resource dependence:
// Author: Jianping.wang
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* R61581_GetOperations(void);
PUBLIC BOOLEAN R61581_Probe(void);

/**************************************************************************************/
// Description: get S6D04H0A driver function
// Global resource dependence:
// Author:
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* S6D04H0A_GetOperations(void);
PUBLIC BOOLEAN S6D04H0A_Probe(void);

/**************************************************************************************/
// Description: get ST7781R driver function
// Global resource dependence:
// Author:
// Note:
/**************************************************************************************/
PUBLIC LCD_OPERATIONS_T* ST7781R_GetOperations(void);
PUBLIC BOOLEAN ST7781R_Probe(void);
/*---------------------------------------------------------------
**   External LCD public variable
-----------------------------------------------------------------*/
extern const LCD_SPEC_T g_lcd_HX8312[];
extern const LCD_SPEC_T g_lcd_HX8346[];
extern const LCD_SPEC_T g_lcd_HX8347[];
extern const LCD_SPEC_T g_lcd_HX8347D[];
extern const LCD_SPEC_T g_lcd_ILI9320;
extern const LCD_SPEC_T g_lcd_ILI9325;
extern const LCD_SPEC_T g_lcd_ILI9325B8;
extern const LCD_SPEC_T g_lcd_ILI9326[];
extern const LCD_SPEC_T g_lcd_ILI9335[];
extern const LCD_SPEC_T g_lcd_LGDP4531[];
extern const LCD_SPEC_T g_lcd_LGDP4532[];
extern const LCD_SPEC_T g_lcd_R61505U[];
extern const LCD_SPEC_T g_lcd_R61509V;
extern const LCD_SPEC_T g_lcd_R61581;
extern const LCD_SPEC_T g_lcd_S6D0139;
extern const LCD_SPEC_T g_lcd_SPDF5408;
extern const LCD_SPEC_T g_lcd_SSD1288[];
extern const LCD_SPEC_T g_lcd_SPFD54124A[];
extern const LCD_SPEC_T g_lcd_SPFD5420A[];
extern const LCD_SPEC_T g_lcd_HX8357;
extern const LCD_SPEC_T g_lcd_ILI9328;
extern const LCD_SPEC_T g_lcd_ILI9225B;
extern const LCD_SPEC_T g_lcd_S6D04H0A;
extern const LCD_SPEC_T g_lcd_ILI9342;
extern const LCD_SPEC_T g_lcd_RM68040;
extern const LCD_SPEC_T g_lcd_ST7781RB8;
extern const LCD_SPEC_T g_lcd_ILI9481;
extern const LCD_SPEC_T g_lcd_ILI9486;
extern const LCD_SPEC_T g_lcd_BYD8817F;
extern const LCD_SPEC_T g_lcd_HX8340B;
extern const LCD_SPEC_T g_lcd_ILI9338B[];
extern const LCD_SPEC_T g_lcd_HX8347G[];
extern const LCD_SPEC_T g_lcd_HX8347G_spi;
extern const LCD_SPEC_T g_lcd_HX8352C;
extern const LCD_SPEC_T g_lcd_R61529;
extern const LCD_SPEC_T g_lcd_ST7789H2;
extern const LCD_SPEC_T g_lcd_GC9304C;
extern const LCD_SPEC_T g_lcd_GC9305;
extern const LCD_SPEC_T g_lcd_GC9306;
extern const LCD_SPEC_T g_lcd_GC9307;
extern const LCD_SPEC_T g_lcd_ST7789v2;
extern const LCD_SPEC_T g_lcd_JD9851;
extern const LCD_SPEC_T g_lcd_GC9a01;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }

#endif  // End of lcm_cfg.c

#endif  // End of _LCM_CFG_H_
