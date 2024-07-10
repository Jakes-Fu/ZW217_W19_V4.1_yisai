/************************************************************************
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

#include "ms_customize_trc.h"
#include "I2c_drv.h"
#include "os_api.h"
#include "fm_drv.h"
#include "Tea5761.h"

#define I2C_WRITE_ADDR  0x20
#define I2C_READ_ADDR   0x21

#define TEA5761_FREQUENCY_GRID	10
#define	TEA5761_ERROR_COUNT	1000

#define	TEA5761_SEARCH_DOWN	0x40
#define	TEA5761_SEARCH_UP	0xC0
#define	TEA5761_PRESET		0x00

#define	TEA5761_IFC_LOW_LIMIT	0x31
#define	TEA5761_IFC_HIGH_LIMIT	0x3E

#define	TEA5761_IFFLAG		(TEA5761_Read_buffer[0]&0x10)
#define	TEA5761_LEVFLAG		(TEA5761_Read_buffer[0]&0x08)
#define	TEA5761_FRRFLAG		(TEA5761_Read_buffer[0]&0x02)
#define	TEA5761_BLFLAG		(TEA5761_Read_buffer[0]&0x01)
#define	TEA5761_AHLSI		(TEA5761_Read_buffer[5]&0x01)
#define	TEA5761_HLSI		(TEA5761_Read_buffer[5]&0x10)
#define	TEA5761_MODE		(TEA5761_Read_buffer[2]&0xC0)
#define	TEA5761_IFCOUNT		(TEA5761_Read_buffer[8]>>1)
#define	TEA5761_RSSI_LEVEL	(TEA5761_Read_buffer[9]>>4)
#define	TEA5761_LD			(TEA5761_Read_buffer[9]&0x08)
#define	TEA5761_PLL_VALUE_GET			((TEA5761_Read_buffer[6]&0x3f)<<8|TEA5761_Read_buffer[7]) 

/*	I2C Read and Write is implemented according target platform	*/
//extern void I2C_Read(unsigned char*,unsigned char Count);
//extern void I2C_Write(unsigned char*, unsigned char);

/*	
	I2C-Byte No. 			REGISTER NAME 	R/W 	ACCESS RESET
------------------------------------------------------------	
	0R - 1R/0W 			INTREG 			R/W 	0000
	2R/1W - 3R/2W 		FRQSET 			R/W		8000
	4R/3W - 5R/4W 		TNCTRL 			R/W 	08D2
	6R - 7R 				FRQCHK 			R 		-
	8R - 9R 				TUNCHK 			R		-
	10R/5W - 11R/6W 	TESTREG 		R/W 	0000
	12R - 13R 			MANID 			R 		004B
	14R - 15R 			CHIPID 			R 		5761
*/
static unsigned char  TEA5761_Write_buffer[7];
static unsigned char  TEA5761_Read_buffer[16];
static TEA5761_CONFIG_STRUCTURE TEA5761_Config_Struc;

static uint32 g_wDisplayBandHigh=10800;
static uint32 g_wDisplayBandLow=8750;
static uint8  g_fm_freq_count = 0;
static uint16 g_fm_autoscan_freq_tab[30];

static void I2C_Write(unsigned char* addr, uint8 cnt)
{
    I2C_WriteCmdArr(I2C_WRITE_ADDR, addr, cnt,SCI_TRUE);

}

static void I2C_Read(unsigned char* addr, uint8 cnt)
{

    I2C_ReadCmdArr(I2C_READ_ADDR, addr, cnt, SCI_FALSE);
	
}

static void  TEA5761_Delay(unsigned short ms)  
{
	/*This function is  no use Temporily and customed by platform*/
}

void TEA5761_Write(void)
{

	I2C_Write(TEA5761_Write_buffer,7);
}

//static void TEA5761_Read(void)
void TEA5761_Read(void)
{
	char i=0;

	for(i=0;i<16;i++)
	{
		TEA5761_Read_buffer[i]=0;
	}
     I2C_Read(TEA5761_Read_buffer,16);
}
static void TEA5761_PLL_Value_Set(unsigned short pll_value,unsigned char mode)
{
	TEA5761_Write_buffer[1]=mode;/*Hold SUD and SM Bit*/
	TEA5761_Write_buffer[1]|=(pll_value&0x3FFF)>>8;
	TEA5761_Write_buffer[2]=(pll_value);
}

/*
	TEA5761's Frequency range is from 76MHz to 108Mhz and Freq value is from 7600 to 10800.
	when HLSI=1, Freq=(pll_value*8192-225000)/10000;
       when HLSI=0, Freq=(pll_value*8192+225000)/10000;
*/
static unsigned short	TEA5761_Calculate_Frequency(unsigned short pll_value,unsigned char HiLo)	
{
	unsigned short RetFreq,temp;
	/* Caculate By Hi Side	*/
	RetFreq= ((unsigned long)pll_value*8192+(HiLo?-225000:225000))/10000;

	temp=RetFreq%10;
	RetFreq=(RetFreq/10)*10;
	if(temp>4) RetFreq+=10;
	//SCI_TRACE_LOW:"TEA5761_Calculate_Frequency()  return=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_135_112_2_18_0_32_55_1460,(uint8*)"d",RetFreq);
	return RetFreq;
}

/*
	TEA5761's PLL value ranged from 0x2458(9304) to 0x339B(13211)
	when HLSI=1(High-Side Injection), pll_value=(4*(Freq*10000 + 225000)/32768);
	when HLSI=0(Low-Side Injection),  pll_value=(4*(Freq*10000 - 225000)/32768);
*/
static unsigned short	TEA5761_Calculate_PLL_Value(unsigned short freq,unsigned char HiLo)	
{
	unsigned short RetPll;
	RetPll = ((unsigned long)freq*10000 + (HiLo?225000:-225000))/8192;
	//SCI_TRACE_LOW:"    TEA5761_Calculate_PLL_Value(freq=%d)  return = 0x%04x    "
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_147_112_2_18_0_32_55_1461,(uint8*)"dd",freq,RetPll);
	return RetPll;
	
}

static void TEA5761_Config(void)
{
	TEA5761_Write_buffer[0]=0x00;
	
	TEA5761_Write_buffer[3] =TEA5761_Config_Struc.Power_UpDown;
	TEA5761_Write_buffer[3]|=TEA5761_Config_Struc.FM_Band;
	TEA5761_Write_buffer[3]|=TEA5761_Config_Struc.Software_Port_1;
	TEA5761_Write_buffer[3]|=TEA5761_Config_Struc.IF_Count_Time;
	TEA5761_Write_buffer[3]|=TEA5761_Config_Struc.Audio_Mute;
	TEA5761_Write_buffer[3]|=TEA5761_Config_Struc.Soft_Mute;
	TEA5761_Write_buffer[3]|=TEA5761_Config_Struc.SNC;

	TEA5761_Write_buffer[4]&=TEA5761_HIGH_SIDE_INJECTION;
	TEA5761_Write_buffer[4]|=TEA5761_Config_Struc.Hard_Mute;
	TEA5761_Write_buffer[4]|=TEA5761_Config_Struc.SearchStop_Level;
	TEA5761_Write_buffer[4]|=TEA5761_Config_Struc.Mono_Stereo;
	TEA5761_Write_buffer[4]|=TEA5761_Config_Struc.Software_Port_2;
	TEA5761_Write_buffer[4]|=TEA5761_Config_Struc.Deemphasis_Time;
	TEA5761_Write_buffer[4]|=TEA5761_Config_Struc.Action_when_IFCount_Fails;
	
	TEA5761_Write_buffer[5] = 0x00|(TEA5761_Config_Struc.Hysteresis_Level);
	TEA5761_Write_buffer[5] = 0x00|(TEA5761_Config_Struc.Trigfr);

	TEA5761_Write_buffer[6] = 0x00;
	
}
static void TEA5761_SetHLSI(unsigned char HiLo)
{
	TEA5761_Write_buffer[4]&=(~TEA5761_HIGH_SIDE_INJECTION);
	TEA5761_Write_buffer[4]|=HiLo;
}
static unsigned char TEA5761_WaitFRRSet(void)
{
	unsigned short i;
	for(i=0;i<TEA5761_ERROR_COUNT;i++)
	{
		TEA5761_Read();
		if (TEA5761_FRRFLAG) 	break;
	}
//	Uart_Printf("\n\tSpend time for waiting FRRFLAG Set:%d",i);
	if(i==TEA5761_ERROR_COUNT) return TEA5761_RET_TIMEOUT;
	return TEA5761_RET_OK;
}

static unsigned char TEA5761_WaitLDSet(void)
{
	unsigned short i;

	for(i=0;i<TEA5761_ERROR_COUNT;i++)
	{
		TEA5761_Read();
		if (TEA5761_LD) 	break;
	}
//	Uart_Printf("\n\tSpend time for waiting LD Set:%d",i);
	if(i==TEA5761_ERROR_COUNT)
	{
		//SCI_TRACE_LOW:"---TEA5761_WaitLDSet(): time out error---"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_208_112_2_18_0_32_55_1462,(uint8*)"");
		return TEA5761_RET_TIMEOUT;
	}
	//SCI_TRACE_LOW:"---TEA5761_WaitLDSet(): return ok---"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_211_112_2_18_0_32_55_1463,(uint8*)"");
	return TEA5761_RET_OK;
}

static void TEA5761_CorrectFreq(unsigned short* Freq)
{
	unsigned char temp;

	//SCI_TRACE_LOW:"----TEA5761_CorrectFreq: freq=%d----"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_219_112_2_18_0_32_55_1464,(uint8*)"d",*Freq);
	if( *Freq>=TEA5761_FREQ_HIGH_LIMIT) 
		{
			*Freq=TEA5761_FREQ_LOW_LIMIT;
		}
	else if(*Freq<=TEA5761_FREQ_LOW_LIMIT) 
		{
			*Freq=TEA5761_FREQ_HIGH_LIMIT;
		}
	else {
		temp=(*Freq)%10;
		(*Freq)=((*Freq)/10)*10;
		if(temp>4) (*Freq)+=10;
	}
	//SCI_TRACE_LOW:"--TEA5761_CorrectFreq(return freq=%d)---"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_233_112_2_18_0_32_56_1465,(uint8*)"d",*Freq);
	
}

unsigned char TEA5761_Toggle_HLSI(void)
{
	unsigned short pll_value,freq;
	unsigned char HiLo;
	TEA5761_Read();
	HiLo=TEA5761_HLSI;
	freq=TEA5761_Calculate_Frequency(TEA5761_PLL_VALUE_GET, HiLo);
	HiLo^=0x10;//Toggle HLSI Bit
	TEA5761_SetHLSI(HiLo);
	pll_value=TEA5761_Calculate_PLL_Value(freq, HiLo);
	TEA5761_PLL_Value_Set(pll_value,TEA5761_PRESET);
	TEA5761_Write();
	return TEA5761_WaitLDSet();	
}
static unsigned char TEA5761_BeginSearch(unsigned char up_down) // 1-up, 0-down
{
	unsigned short pll_value,freq;
	unsigned char HiLo;
	TEA5761_Read();
	freq=TEA5761_Calculate_Frequency(TEA5761_PLL_VALUE_GET,TEA5761_HLSI);
	if (up_down==TEA5761_SEARCH_UP)
		{
			HiLo=TEA5761_LOW_SIDE_INJECTION;
			freq+=TEA5761_FREQUENCY_GRID;
		}
	else	 if(up_down==TEA5761_SEARCH_DOWN)
		{
			HiLo=TEA5761_HIGH_SIDE_INJECTION;
			freq-=TEA5761_FREQUENCY_GRID;
		}
	else return TEA5761_RET_INVALID_PARAMETER;

	TEA5761_SetHLSI(HiLo);
	TEA5761_CorrectFreq(&freq);
	pll_value=TEA5761_Calculate_PLL_Value(freq, HiLo);
	TEA5761_PLL_Value_Set(pll_value,up_down);
	
	TEA5761_Write();
	return TEA5761_WaitFRRSet();
}

/*
	This function command TEA5761 lock on  the preset frequency, 
	If you transfer a value out of 8750~10800 to parameter [Freq],this function will correct the freq value first
  */
uint32 TEA5761_Preset(unsigned short Freq)
{
	unsigned short pll_value;
	unsigned char Low_Level,High_Level,HiLo;

	//SCI_TRACE_LOW:"----TEA5761_Preset(%d)----"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_286_112_2_18_0_32_56_1466,(uint8*)"d",Freq);

	TEA5761_CorrectFreq(&Freq);
	TEA5761_SetHLSI(TEA5761_HIGH_SIDE_INJECTION);
	/*	with HLSI=HiSide measureing the level at Freq + and - 450Khz to determin the correct HLSI Value	*/

	/*	Measuer Freq + 450K */
	pll_value=TEA5761_Calculate_PLL_Value(Freq+45,TEA5761_HIGH_SIDE_INJECTION);
	TEA5761_PLL_Value_Set(pll_value,TEA5761_PRESET);
	TEA5761_Write();
	if(TEA5761_WaitLDSet()!=TEA5761_RET_OK) return TEA5761_RET_TIMEOUT;
	High_Level=TEA5761_RSSI_LEVEL;

	/*	Measuer Freq - 450K */
	TEA5761_SetHLSI(TEA5761_LOW_SIDE_INJECTION);
	pll_value=TEA5761_Calculate_PLL_Value(Freq-45,TEA5761_LOW_SIDE_INJECTION);
	TEA5761_PLL_Value_Set(pll_value,TEA5761_PRESET);
	TEA5761_Write();
	if(TEA5761_WaitLDSet()!=TEA5761_RET_OK) return TEA5761_RET_TIMEOUT;
	Low_Level=TEA5761_RSSI_LEVEL;

	if(High_Level < Low_Level)
		{
		HiLo=TEA5761_LOW_SIDE_INJECTION;
	}
	else
		{
		HiLo=TEA5761_HIGH_SIDE_INJECTION;
	}
	TEA5761_SetHLSI(HiLo);
	pll_value=TEA5761_Calculate_PLL_Value(Freq,HiLo);
	TEA5761_PLL_Value_Set(pll_value,TEA5761_PRESET);
	TEA5761_Write();
	return TEA5761_WaitLDSet();
}



/*	
	This Function mainly configure TEA5761's TNCTRL Register, 
	please refer the comment of structure TEA5761_CONFIG_STRUCTURE in Tea5761.h
	
*/
unsigned char TEA5761_Configure(TEA5761_CONFIG_STRUCTURE* pConfigStruc)
{
	if(pConfigStruc) 
		{
			TEA5761_Config_Struc=*pConfigStruc;
		}
	
	TEA5761_Config();
	TEA5761_Write();
	if(TEA5761_Config_Struc.Power_UpDown==TEA5761_POWER_OFF) return TEA5761_RET_OK;
	if(TEA5761_WaitFRRSet()!=TEA5761_RET_OK) return TEA5761_RET_TIMEOUT;
	return TEA5761_RET_OK;
}

/*
	This function can get some necessary information to user, 
	[pConfigStruc] will return  Current Configuration;
	[pStatusStruc] will return  Current Frequency, Current IF Count , Current Signal level and so on
	please refer to the comment of structure TEA5761_STATUS_DATA_STRUCTURE and TEA5761_CONFIG_STRUCTURE in Tea5761.h
	
*/
unsigned char TEA5761_GetStatus(TEA5761_CONFIG_STRUCTURE * pConfigStruc,TEA5761_STATUS_DATA_STRUCTURE* pStatusStruc)
{
	if(!(pConfigStruc||pStatusStruc)) return TEA5761_RET_NULLPOINTER;
	TEA5761_Read();
	if(pStatusStruc)
		{
			pStatusStruc->Frequency=TEA5761_Calculate_Frequency(TEA5761_PLL_VALUE_GET,TEA5761_HLSI);
			pStatusStruc->IF_Count=(unsigned short)TEA5761_IFCOUNT*4096/1000;
			//SCI_TRACE_LOW:"--if_count=%d----"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_356_112_2_18_0_32_56_1467,(uint8*)"d",TEA5761_IFCOUNT);
			pStatusStruc->Signal_Level=TEA5761_RSSI_LEVEL*100/15;//by percent
			pStatusStruc->Stereo_Mono_Blend=(TEA5761_Read_buffer[9]&0x04)>>2;
			pStatusStruc->Version=TEA5761_Read_buffer[12]>>12;
			pStatusStruc->Main_ID=(TEA5761_Read_buffer[12]<<4)>>5;
			pStatusStruc->Chip_ID=(TEA5761_Read_buffer[14]<<8)|(TEA5761_Read_buffer[15]);
		}
	if(pConfigStruc)
		{
		//*pConfigStruc=TEA5761_Config_Struc;
		pConfigStruc->Power_UpDown=TEA5761_Read_buffer[4]&TEA5761_POWER_ON;
		pConfigStruc->FM_Band=TEA5761_Read_buffer[4]&TEA5761_JAPAN_BAND;
		pConfigStruc->IF_Count_Time=TEA5761_Read_buffer[4]&TEA5761_15_625_ms;
		pConfigStruc->Software_Port_1=TEA5761_Read_buffer[4]&TEA5761_SWP1_FRRFLAG;
		pConfigStruc->Audio_Mute=TEA5761_Read_buffer[4]&TEA5761_AFM_MUTE;
		pConfigStruc->Soft_Mute=TEA5761_Read_buffer[4]&TEA5761_SOFTMUTE_ON;
		pConfigStruc->SNC=TEA5761_Read_buffer[4]&TEA5761_SNC_ON;

		pConfigStruc->Hard_Mute=TEA5761_Read_buffer[5]&TEA5761_HARD_MUTE_ON;
		pConfigStruc->SearchStop_Level=TEA5761_Read_buffer[5]&TEA5761_SSL_LEVEL_10;
		
		pConfigStruc->HiLo_Injection=TEA5761_HLSI;
		pConfigStruc->Mono_Stereo=TEA5761_Read_buffer[5]&TEA5761_MONO;
		pConfigStruc->Software_Port_2=TEA5761_Read_buffer[5]&TEA5761_SWP2_HIGH;
		pConfigStruc->Deemphasis_Time=TEA5761_Read_buffer[5]&TEA5761_50_uS;
		pConfigStruc->Action_when_IFCount_Fails=TEA5761_Read_buffer[5]&TEA5761_AHLSI_STOP;

		pConfigStruc->Hysteresis_Level=TEA5761_Read_buffer[10]&TEA5761_LARGE_HYSTERESIS_LEVEL;
	}
	return TEA5761_RET_OK;
}

/*
	This function will initialize TEA5761 and must be called first of all
*/
uint32 TEA5761_Initial(void)
{    

	unsigned short freq=10770;
	//SCI_TRACE_LOW:"----TEA5761_Initial(freq=%d)----"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_394_112_2_18_0_32_56_1468,(uint8*)"d",freq);
       Open32kClock( SCI_TRUE );
	OpenFMPA(SCI_TRUE);
	
	/* TNCCONTROL	*/
	TEA5761_Config_Struc.Power_UpDown=TEA5761_POWER_ON;/*	*/
	TEA5761_Config_Struc.FM_Band=TEA5761_CHINA_BAND;
	TEA5761_Config_Struc.IF_Count_Time=TEA5761_15_625_ms;
	TEA5761_Config_Struc.Software_Port_1=TEA5761_SWP1_FRRFLAG;
	TEA5761_Config_Struc.Audio_Mute=TEA5761_AFM_UNMUTE;
	TEA5761_Config_Struc.Soft_Mute=TEA5761_SOFTMUTE_ON;
	TEA5761_Config_Struc.SNC=TEA5761_SNC_ON;

	TEA5761_Config_Struc.Hard_Mute=TEA5761_HARD_MUTE_OFF;
	TEA5761_Config_Struc.SearchStop_Level=TEA5761_SSL_LEVEL_5;
	TEA5761_Config_Struc.Mono_Stereo=TEA5761_STEREO;
	TEA5761_Config_Struc.Software_Port_2=TEA5761_SWP2_LOW;
	TEA5761_Config_Struc.Deemphasis_Time=TEA5761_50_uS;
	TEA5761_Config_Struc.Action_when_IFCount_Fails=TEA5761_AHLSI_CONTINUE;

	TEA5761_Config_Struc.Hysteresis_Level=TEA5761_SMALL_HYSTERESIS_LEVEL;
	TEA5761_Config_Struc.Trigfr	= 	TEA5761_XTAL_PIN;
	TEA5761_SetHLSI(TEA5761_HIGH_SIDE_INJECTION);
	TEA5761_Config();
	return 	TEA5761_Preset(freq);

}


/*
	This function will command TEA5761 to search channel in the [dicrection]
	if [direction] == 0, search down 
	if [direction] == 1, search up
*/
unsigned char TEA5761_Search(unsigned char direction)
{
	unsigned short i,freq;
	unsigned char RetValue,up_down,IF_Count,ReachLimit_Count=0;
	char level_1,level_2;

	//SCI_TRACE_LOW:"---Tea5761_search(%d)---"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_433_112_2_18_0_32_56_1469,(uint8*)"d",direction);
	TEA5761_Read();
	freq=TEA5761_Calculate_Frequency(TEA5761_PLL_VALUE_GET,TEA5761_HLSI);
	TEA5761_Config_Struc.Hard_Mute=TEA5761_HARD_MUTE_ON;
	TEA5761_Config();
	if(direction==0) 
		{
		up_down=TEA5761_SEARCH_DOWN;
		}
	else if(direction==1) 
		{
		up_down=TEA5761_SEARCH_UP;
		}
	else return TEA5761_RET_INVALID_PARAMETER;

	for(i=0;i<TEA5761_ERROR_COUNT/10;i++)
	{
		RetValue=TEA5761_BeginSearch(up_down);
		if(RetValue==TEA5761_RET_TIMEOUT) break;
		if(TEA5761_BLFLAG) {
	//		Uart_Printf("\n\t##Reach band Limit");
				ReachLimit_Count++;
				if(ReachLimit_Count>1)
					{
						RetValue = TEA5761_RET_REACH_BANDLIMIT;
						break;
					}
				continue;
			}
		if(TEA5761_AHLSI)
			{
			TEA5761_Read();
			IF_Count=TEA5761_IFCOUNT;
			//SCI_TRACE_LOW:"---if_count[1]=%d----"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_466_112_2_18_0_32_56_1470,(uint8*)"d",IF_Count);
			if(IF_Count<TEA5761_IFC_LOW_LIMIT|| IF_Count>TEA5761_IFC_HIGH_LIMIT)
				{
					continue;//IF Count is incorrect, so continue
				}
			else
				{
					level_1=TEA5761_RSSI_LEVEL;
					TEA5761_Toggle_HLSI();
					level_2=TEA5761_RSSI_LEVEL;
					IF_Count=TEA5761_IFCOUNT;
					//SCI_TRACE_LOW:"---if_count[2]=%d----"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_477_112_2_18_0_32_56_1471,(uint8*)"d",IF_Count);
					if((level_1-level_2<2)&&(level_1-level_2>-2)&&(IF_Count>=TEA5761_IFC_LOW_LIMIT)&&(IF_Count<=TEA5761_IFC_HIGH_LIMIT))
					{
						TEA5761_Toggle_HLSI();
						break;
					}	
				}
			}
		else
			{
			break;
		}
	}	
	
	if(RetValue==TEA5761_RET_OK)
		{
		TEA5761_Read();
		freq=TEA5761_Calculate_Frequency(TEA5761_PLL_VALUE_GET,TEA5761_HLSI);
		}
	TEA5761_Config_Struc.Hard_Mute=TEA5761_HARD_MUTE_OFF;
	TEA5761_Config();
	TEA5761_Preset(freq);
	//SCI_TRACE_LOW:"---Tea5761_Search...return----"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_499_112_2_18_0_32_56_1472,(uint8*)"");
	return RetValue;
}

LOCAL uint32 TEA5761_PlayStart(uint16 freq)
{
	//SCI_TRACE_LOW:"TEA5761_palystart()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_505_112_2_18_0_32_56_1473,(uint8*)"");
	Open32kClock(SCI_TRUE);
	TEA5761_Config_Struc.Power_UpDown = TEA5761_POWER_ON;
	TEA5761_Config_Struc.Soft_Mute=TEA5761_SOFTMUTE_OFF;
	TEA5761_Config();
	TEA5761_Write();
	TEA5761_Preset(freq*10);
	return SCI_TRUE;
}

LOCAL uint32 TEA5761_PlayStop()
{
	//SCI_TRACE_LOW:"TEA5761_palystop()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_517_112_2_18_0_32_56_1474,(uint8*)"");

	Open32kClock(SCI_FALSE);	
	TEA5761_Config_Struc.Soft_Mute=TEA5761_SOFTMUTE_ON;	
	TEA5761_Config_Struc.Power_UpDown = TEA5761_POWER_OFF;
	TEA5761_Config();
	TEA5761_Write();
	return SCI_TRUE;
}


LOCAL void TEA5761_Open(void)
{
	//SCI_TRACE_LOW:"TEA5761_Open()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_530_112_2_18_0_32_56_1475,(uint8*)"");
	OpenFMPA(SCI_TRUE);
       Open32kClock(SCI_TRUE);
}

LOCAL void TEA5761_Close(void)
{
	//SCI_TRACE_LOW:"TEA5761_close()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TEA5761_537_112_2_18_0_32_56_1476,(uint8*)"");
     TEA5761_PlayStop();
     OpenFMPA(SCI_FALSE);

}

	
LOCAL uint32 TEA5761_ManualSeek(uint16 freq,
                                            FM_SCAN_DIR_E  seek_dir, 
                                            uint8 steps, 
                                            FM_SINGLE_CHANNEL_INFO_T *single_channel_info)
{
      uint16 dest_freq;
      uint8 i=steps;
	TEA5761_CONFIG_STRUCTURE configstruct;

	switch( seek_dir )
	{
		case FM_SCAN_UP:
			{
				dest_freq = (freq+i)*10;
			}	
			break;
		case FM_SCAN_DOWN:
			{
				dest_freq = (freq-i)*10;
			}	
			break;
		default:
			break;
	}
		
	TEA5761_CorrectFreq(&dest_freq);
	
	TEA5761_Preset( dest_freq );

      single_channel_info->freq = dest_freq/10;

	TEA5761_GetStatus(&configstruct,NULL);


	if( TEA5761_STEREO==configstruct.Mono_Stereo)
	{
	      single_channel_info->is_stereo = STEREO_MODE;	
	}
	else
	{
	      single_channel_info->is_stereo = MONO_MODE;
	}
	return SCI_TRUE;
}

LOCAL uint32 TEA5761_AutoSeek( uint16 freq, 
                                         FM_SCAN_DIR_E seek_dir,
                                         FM_SCAN_MODE_E mode, 
                                         FM_ALL_CHANNEL_INFO_T  *all_channel_info )
{
    uint8  scan_param = 0;
    uint8 i;
    TEA5761_STATUS_DATA_STRUCTURE statusstruct={0};
	
	if(FM_SCAN_UP == seek_dir)
	{
	    scan_param |= 0x01;	
	}

	TEA5761_Preset(freq*10);

	TEA5761_Search(scan_param);	
       TEA5761_GetStatus(NULL,&statusstruct);
	
       all_channel_info->freq_count = g_fm_freq_count;
       all_channel_info->auto_freq_tab_ptr = &g_fm_autoscan_freq_tab[0];

      	 if(FM_SCAN_ONE == mode)
      {
          all_channel_info->freq_count = 1;
          all_channel_info->auto_freq_tab_ptr[0]=statusstruct.Frequency/10;
      	}

	return SCI_TRUE;
}

LOCAL uint32 TEA5761_SetVol(FM_VOL_LEVEL_E vol_level)
{

	return SCI_TRUE;
}

LOCAL uint32  TEA5761_SetFMRegion(FM_REGION_E  region)
{
	switch(region)
	{
		case FM_REGION_US:
			TEA5761_Config_Struc.FM_Band=TEA5761_CHINA_BAND;
			g_wDisplayBandHigh=1080;
			g_wDisplayBandLow=8750;
			break;
		case FM_REGION_JAPAN:
			TEA5761_Config_Struc.FM_Band=TEA5761_JAPAN_BAND;
			g_wDisplayBandHigh=9000;
			g_wDisplayBandLow=7600;
			break;			
		default:
			break;
	}
	TEA5761_Config();
	TEA5761_Write();
	return SCI_TRUE;
}

LOCAL uint32 TEA5761_SetScanLevel(FM_SCANLEVEL_E level)
{
	switch( level )
	{
		case FM_SCANLEVEL_LOW:
			//TEA5761_Config_Struc.SearchStop_Level=TEA5761_SSL_LEVEL_5;
			TEA5761_Config_Struc.SearchStop_Level=TEA5761_SSL_LEVEL_3;
			break;
		case FM_SCANLEVEL_MID:
			//TEA5761_Config_Struc.SearchStop_Level=TEA5761_SSL_LEVEL_7;
			TEA5761_Config_Struc.SearchStop_Level=TEA5761_SSL_LEVEL_5;
			break;
		case FM_SCANLEVEL_HIGH:
			//TEA5761_Config_Struc.SearchStop_Level=TEA5761_SSL_LEVEL_10;
			TEA5761_Config_Struc.SearchStop_Level=TEA5761_SSL_LEVEL_7;
			break;
		default:
			break;
	}

	TEA5761_Config();
	TEA5761_Write();
	return SCI_TRUE;
}
  
//Add by luther.li 2012-12-11 
/*****************************************************************************/
//  FUNCTION:     TEA5761_FM_Ioctl
//  Description:  This function will contrl fm command
//  return:     
//  Author:         
//  date:  
//	Note:        
/*****************************************************************************/	 
LOCAL uint32 TEA5761_FM_Ioctl(uint32 cmd, void *data)
{
    return 0;
}


//FM operations set
PUBLIC FM_OPERATION_T g_Tea5761_ops=
{
    TEA5761_Initial,   
    TEA5761_Open,     
    TEA5761_Close ,   

    TEA5761_PlayStart,
    TEA5761_PlayStop,

    TEA5761_ManualSeek,
    TEA5761_AutoSeek,

    TEA5761_SetScanLevel,
    TEA5761_SetVol,

    TEA5761_SetFMRegion,
    TEA5761_FM_Ioctl
};
