/****************************************************************************

 *        Copyright 2005 World Peace Group(China). All Rights Reserved.
 *										
 *  This software is copyrighted by and is the property of World Peace Group(China).
 *  All rights are reserved by World Peace Group(China)
 *  This software may only be used in accordance with the	
 *  corresponding license agreement. Any unauthorized use, duplication,		
 *  distribution, or disclosure of this software is expressly forbidden.
 *																
 *  This Copyright notice MUST not be removed or modified without prior		
 *  written consent of World Peace Group(China)							
 *																			
 *  World Peace Group reserves the right to modify this			
 *  software without notice.												
 *																		
 *  	Author: 				Snow Hsu(TME of WPG)														
 *	Create Date:			17:42 pm, Dec 16th,2005
 *	Last Modified Date:		Dec 16th,2005
 *
 *
 *
 *	The TEA5761 Driver totally contain 2 files:Tea5761.c and Tea5761.h
 *	Tea5761.c mainly inlcude the function definition 
 *	
 *	Tea5761.h is the macro definition the programmer may use and the API's declaration
 *	
 ***************************************************************************/
#ifndef _TEA5761_H
#define	_TEA5761_H



/*-----------------------------------------
	China,US,Europe	87.5Mhz~108Mhz(8750~10800)
	Japan			76Mhz~91Mhz(7600~9100)
---------------------------------------------*/
#define	TEA5761_FREQ_LOW_LIMIT	8750
#define	TEA5761_FREQ_HIGH_LIMIT	10800

/*	Return Error Code	*/
#define	TEA5761_RET_OK 						0x00
#define	TEA5761_RET_NULLPOINTER 				0x01
#define	TEA5761_RET_TIMEOUT					0x02
#define	TEA5761_RET_REACH_BANDLIMIT			0x04
#define	TEA5761_RET_INVALID_PARAMETER		0x08


/*	
	The below macro definition is mainly for TEA5761_Configure() and TEA5761_GetStatus() 's parameters
 	Please don't change them 	
 */
/*	for TEA5761_CONFIG_STRUCTURE.Power_UpDown	*/
#define	TEA5761_POWER_OFF	0x00
#define	TEA5761_POWER_ON		0x40

/*	for TEA5761_CONFIG_STRUCTURE.FM_Band	*/
#define	TEA5761_CHINA_BAND	0x00
#define	TEA5761_JAPAN_BAND	0x20

/*	for TEA5761_CONFIG_STRUCTURE Software_Port_1	*/
#define	TEA5761_SWP1_SWP2		0x00
#define	TEA5761_SWP1_FRRFLAG		0x10

/*	for TEA5761_CONFIG_STRUCTURE.IF_Count_Time	*/
#define	TEA5761_1_953_ms		0x00
#define	TEA5761_15_625_ms	0x08

/*	for TEA5761_CONFIG_STRUCTURE.Audio_Mute		*/
#define	TEA5761_AFM_MUTE		0x04
#define	TEA5761_AFM_UNMUTE	0x00

/*	for TEA5761_CONFIG_STRUCTURE.Softmute	*/
#define	TEA5761_SOFTMUTE_ON	0x02
#define	TEA5761_SOFTMUTE_OFF	0x00

/*	for TEA5761_CONFIG_STRUCTURE.SNC	*/
#define	TEA5761_SNC_ON		0x01
#define	TEA5761_SNC_OFF		0x00

/*	for TEA5761_CONFIG_STRUCTURE.Har_Mute	*/
#define	TEA5761_HARD_MUTE_ON	0x80
#define	TEA5761_HARD_MUTE_OFF	0x00

/*	for TEA5761_CONFIG_STRUCTURE.SearchStop_Level	*/
#define	TEA5761_SSL_LEVEL_3	0x00
#define	TEA5761_SSL_LEVEL_5	0x20
#define	TEA5761_SSL_LEVEL_7	0x40
#define	TEA5761_SSL_LEVEL_10	0x60

/*	for TEA5761_CONFIG_STRUCTURE.HiLo_Injection	*/
#define	TEA5761_LOW_SIDE_INJECTION	0x00
#define	TEA5761_HIGH_SIDE_INJECTION	0x10

/*	for TEA5761_CONFIG_STRUCTURE.Stere_Mono	*/
#define	TEA5761_MONO		0x08
#define	TEA5761_STEREO	0x00

#define  STEREO_MODE  		0x01
#define  MONO_MODE			0x00
/*	for TEA5761_CONFIG_STRUCTURE.Software_Port_2 	*/
#define	TEA5761_SWP2_HIGH	0x04
#define	TEA5761_SWP2_LOW	0x00

/*	for TEA5761_CONFIG_STRUCTURE.Deemphasis_Time 	*/
#define	TEA5761_50_uS		0x02
#define	TEA5761_75_uS		0x00

/*	for TEA5761_CONFIG_STRUCTURE.Action_when_IF_Fails 	*/
#define	TEA5761_AHLSI_STOP		0x01
#define	TEA5761_AHLSI_CONTINUE	0x00

/*	for TEA5761_CONFIG_STRUCTURE.Hysteresis_Level 	*/
#define TEA5761_LARGE_HYSTERESIS_LEVEL	0x10
#define TEA5761_SMALL_HYSTERESIS_LEVEL	0x00

//fly add
#define TEA5761_XTAL_PIN		0x08
#define TEA5761_FREQIN_PIN		0x00


/*	typed definition	*/
typedef struct TEA5761_Config_Structure
{
/* TNCCONTROL_1	4R/3W*/
unsigned char	 Power_UpDown;		/*	Bit 6*/
unsigned char  FM_Band;			/*	Bit 5*/
unsigned char  Software_Port_1;	/*	Bit 4*/
unsigned char  IF_Count_Time;		/*	Bit 3*/


unsigned char  Audio_Mute;	/*	Bit 2*/
unsigned char  Soft_Mute;		/*	Bit 1*/
unsigned char  SNC;			/*	Bit 0*/

/*	TNCCONTROL_2  5R/4W	*/
unsigned char  Hard_Mute;			/*	Bit 7*/
unsigned char  SearchStop_Level;	/*	Bit 6~5*/
unsigned char  HiLo_Injection;		/*	Bit 4*/

unsigned char  Mono_Stereo;		/*	Bit 3*/
unsigned char  Software_Port_2;	/*	Bit 2*/
unsigned char  Deemphasis_Time;	/*	Bit 1*/
unsigned char  Action_when_IFCount_Fails;/*	Bit 0*/

unsigned char Hysteresis_Level;	/*10R/5W	Bit 4*/

//fly add
unsigned char Trigfr;			/* 0 XTAL as frequency in; 1 select FREQIN */

}TEA5761_CONFIG_STRUCTURE;

typedef struct TEA5761_StatusData_Structure
{
	unsigned short Frequency;			/*	7600~10800	*/
	unsigned char IF_Count;			/*	unit:KHz, campared with 225K*/
	unsigned char Signal_Level;		/* 0%~100%	*/
	unsigned char Stereo_Mono_Blend;	/*	1:Blend	0:NoBlend	*/
	unsigned char Version;
	unsigned char Main_ID;
	unsigned short Chip_ID;
}TEA5761_STATUS_DATA_STRUCTURE;

extern uint32 TEA5761_Initial(void);
extern unsigned char TEA5761_Configure(TEA5761_CONFIG_STRUCTURE * pConfigStruc);
extern unsigned char TEA5761_GetStatus(TEA5761_CONFIG_STRUCTURE * pConfigStruc, TEA5761_STATUS_DATA_STRUCTURE * pStausStruc);
extern unsigned char TEA5761_Search(unsigned char direction);
extern uint32 TEA5761_Preset(unsigned short Freq);
extern unsigned char TEA5761_Toggle_HLSI(void);/*Please don't care this function*/
extern void TEA5761_Write(void);
extern void TEA5761_Read(void);
#endif
