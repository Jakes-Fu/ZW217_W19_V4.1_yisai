/****************************************************************************************
** File Name:      tft_S6D04H0A.h                                           			*
** DATE:           23/11/2010                                                			*
** Copyright:      2010 Samsung Electronics. All Rights Reserved.         			*
** Description:    this file defines S6D04H0A LCD interface,LCDC MCU MODE			*
*****************************************************************************************

*****************************************************************************************
**                         Edit History                                       			*
**--------------------------------------------------------------------------------------*
** DATE                  NAME			  DESCRIPTION               					*
** 24/11/2010     	     tiebing.guo        Create.										*
*****************************************************************************************/

#define DELAY_SEQ       0xFFFD
#define END_SEQ         0xFFFE
#define END_SEQ 		0xFFFE
#define LCD_CMD 		0
#define LCD_PRM 		1
#define PRM_NUM 		2				
/**--------------------------------------------------------------------------*
 **                         Constant DEFINITION                              *
 **--------------------------------------------------------------------------*/ 


/**--------------------------------------------------------------------------*
 **                         Array of command and parameter                              *
 **--------------------------------------------------------------------------*/ 
__align(32) UInt16 tftDrvS6D04H0_PowerOn_Seq_Set[][2] =
{
	/* Level 2 command access */
	LCD_CMD,	0xF0,
	PRM_NUM,	2,
	LCD_PRM,	0x5A,
	LCD_PRM,	0x5A,

	/* Power Setting Sequence */
	LCD_CMD,	0xF4,
	PRM_NUM,	11,
	LCD_PRM,	0x0B,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x22,
	LCD_PRM,	0x67,
	LCD_PRM,	0x04,
	LCD_PRM,	0x02,
	LCD_PRM,	0x2A,
	LCD_PRM,	0x7A,
	LCD_PRM,	0x05,

	LCD_CMD,	0xF5,
	PRM_NUM,	6,
	LCD_PRM,	0x00,
	LCD_PRM,	0x55,
	LCD_PRM,	0x37,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	LCD_CMD,	0xF6,
	PRM_NUM,	9,
	LCD_PRM,	0x07,
	LCD_PRM,	0x01,
	LCD_PRM,	0x07,
	LCD_PRM,	0x00,
	LCD_PRM,	0x02,
	LCD_PRM,	0x0C,
	LCD_PRM,	0x02,
	LCD_PRM,	0x08,
	LCD_PRM,	0x05,

	/* Initialising Sequence */
	LCD_CMD,	0x35,
	PRM_NUM,	1,
	LCD_PRM,	0x00,

	LCD_CMD,	0x36,
	PRM_NUM,	1,
	LCD_PRM,	0x48,

	LCD_CMD,	0x3A,
	PRM_NUM,	1,
	LCD_PRM,	0x05,

	LCD_CMD,	0xF7,
	PRM_NUM,	2,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	LCD_CMD,	0xF2,
	PRM_NUM,	12,
	LCD_PRM,	0x28,
	LCD_PRM,	0x65,
	LCD_PRM,	0x03,
	LCD_PRM,	0x02,
	LCD_PRM,	0x02,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x21,
	LCD_PRM,	0x48,
	LCD_PRM,	0x00,
	LCD_PRM,	0x03,
	LCD_PRM,	0x01,

	/* Gamma Setting Sequence */
	/* RED */
	LCD_CMD,	0xF9,
	PRM_NUM,	1,
	LCD_PRM,	0x04,

	LCD_CMD,	0xFA,
	PRM_NUM,	16,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x01,
	LCD_PRM,	0x18,
	LCD_PRM,	0x28,
	LCD_PRM,	0x3F,
	LCD_PRM,	0x29,
	LCD_PRM,	0x29,
	LCD_PRM,	0x24,
	LCD_PRM,	0x28,
	LCD_PRM,	0x2A,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	LCD_CMD,	0xFB,
	PRM_NUM,	16,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x1E,
	LCD_PRM,	0x21,
	LCD_PRM,	0x21,
	LCD_PRM,	0x26,
	LCD_PRM,	0x14,
	LCD_PRM,	0x3C,
	LCD_PRM,	0x27,
	LCD_PRM,	0x18,
	LCD_PRM,	0x05,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	/* GREEN */
	LCD_CMD,	0xF9,
	PRM_NUM,	1,
	LCD_PRM,	0x02,

	LCD_CMD,	0xFA,
	PRM_NUM,	16,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x01,
	LCD_PRM,	0x1A,
	LCD_PRM,	0x30,
	LCD_PRM,	0x3F,
	LCD_PRM,	0x2B,
	LCD_PRM,	0x24,
	LCD_PRM,	0x23,
	LCD_PRM,	0x25,
	LCD_PRM,	0x2A,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	LCD_CMD,	0xFB,
	PRM_NUM,	16,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x1E,
	LCD_PRM,	0x20,
	LCD_PRM,	0x20,
	LCD_PRM,	0x21,
	LCD_PRM,	0x10,
	LCD_PRM,	0x3D,
	LCD_PRM,	0x29,
	LCD_PRM,	0x19,
	LCD_PRM,	0x05,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	/* BLUE */
	LCD_CMD,	0xF9,
	PRM_NUM,	1,
	LCD_PRM,	0x01,

	LCD_CMD,	0xFA,
	PRM_NUM,	16,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x01,
	LCD_PRM,	0x1A,
	LCD_PRM,	0x2D,
	LCD_PRM,	0x3F,
	LCD_PRM,	0x2A,
	LCD_PRM,	0x26,
	LCD_PRM,	0x25,
	LCD_PRM,	0x26,
	LCD_PRM,	0x2A,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	LCD_CMD,	0xFB,
	PRM_NUM,	16,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x1E,
	LCD_PRM,	0x21,
	LCD_PRM,	0x22,
	LCD_PRM,	0x23,
	LCD_PRM,	0x11,
	LCD_PRM,	0x3D,
	LCD_PRM,	0x2C,
	LCD_PRM,	0x19,
	LCD_PRM,	0x05,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,
	LCD_PRM,	0x00,

	/* Level 2 command blocking */
	LCD_CMD,	0xF0,
	PRM_NUM,	2,
	LCD_PRM,	0xA5,
	LCD_PRM,	0xA5,

	/* Sleep out */
	LCD_CMD,	0x11,
	PRM_NUM,	0,

	DELAY_SEQ,	160,

	/* Display On */
	LCD_CMD,	0x29,
	PRM_NUM,	0,

	END_SEQ, 0x0000,
}