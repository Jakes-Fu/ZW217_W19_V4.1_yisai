/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmiapi.h
**		This module defines Nmi Driver APIs interface.
**	
** 
*******************************************************************************/
#ifndef _NMI_API_H_
#define _NMI_API_H_

/******************************************************************************
**
**	Nmi Atv/FM Apis Return Code 
**
*******************************************************************************/

typedef enum {
	NMI_RET_OK = 1,
	NMI_RET_ERR_IO = -1,
	NMI_RET_BAD_PARAMETER
} tNmiRetCode;

/******************************************************************************
**
**	Nmi Atv/FM Command Codes Defines
**		NMI_DRV_INIT: This command code is used to initialize the chipset.
**			Input/Output Parameters: A type of tAtvInit which includes the following fields:
**				xo (input): the crystal used which can be 26 or 27 MHz,
**				format (input): the BT601 format,
**				rate (input): the BT601 rate,
**				clk_polarity (input): Set to 1 to reverse clock polarity,
**				vsync_polarity (input): Set to 1 to reverse vsync polarity,
**				hsync_polarity (input): Set to 1 to reverse hsync polarity,
**				input_order (input): 0: UYVY; 1: YUYV,
**				blank_interval (input): 
**
*******************************************************************************/
#define NMI_DRV_INIT							0x0

typedef enum {
	nXO26MHz = 0,
	nXO27MHz
} tAtvXo;

typedef enum {
	nRGB = 0,
	nYUV,
	nYCbCr
} tAtvBTFormat;

typedef enum {
	nBT27MHz = 0,
	nBT13_5MHz,
	nBT6_75MHz
} tAtvBTRate;

#if 1
typedef enum {
	bt320x240=0,
	bt80x60,
	bt176x144,
	bt220x176,
	bt240x180,
	bt426x320,
	bt360x240,
	bt480x280,
	bt480x360
} tBtSize;
#endif

typedef enum {
	IO_2_8Volt = 0,
	IO_1_8Volt
} tIOVoltage;

typedef struct {
	tAtvXo xo;
	tAtvBTFormat format;
	tBtSize size;	
	tIOVoltage io_volt;
	tAtvBTRate rate;
	int clk_polarity;
	int vsync_polarity;
	int hsync_polarity;
	int input_order;
	int blank_interval;
	int interlace;
	int half_frame_rate;
	int enable_pattern;
} tAtvInit;

/******************************************************************************
**
**	Nmi Atv/FM Command Codes Defines
**		NMI_DRV_DEINIT: This command code is used to de-initialize the chipset.
**			Input Parameters: None.
**
*******************************************************************************/

#define NMI_DRV_DEINIT						0x1

/******************************************************************************
**
**	Nmi Atv/FM Command Codes Defines
**		NMI_DRV_SET_REGION: This command code is used to set the region.
**			Input/Output Parameters: A type of tAtvRegion which includes the following fields:
**				tNmiRegion (input): A type defines by Nmi for each region or country,
**				schannel (output): Start channel of this country; it is returned by the driver,
**				echannel (output): End channel of this country; it is returned by the driver, 								
**				nchannel (output): Number of channels.
**
*******************************************************************************/

#define NMI_DRV_SET_REGION			0x2

typedef enum {
#if 1
	nChina = 0            
	,nHongKong             
	,nChinaSz              
	,nCambodia             
	,nCanada               
	,nChile                
	,nAfghanistan          
	,nArgentina            
	,nBrazil               
	,nIndonesia            
	,nJapan                
	,nKorea                
	,nLaos                 
	,nMalaysia             
	,nMexico               
	,nMyanmar              
	,nPhilippines          
	,nSingapore            
	,nSAfrica          
	,nTaiwan               
	,nThailand             
	,nTurkey               
	,nUAE                  
	,nUk                
	,nUsa                
	,nVenezuela            
	,nVietnam              
	,nWesternEurope        
	,nSpain                
	,nPortugal             
	,nPakistan             
	,nIndia                
	,nAustralia            
	,nParagury             
	,nUruguay              
                 
	,nBulgaria//SECAM D/K  
	,nCongo//SECAM D       
	,nEgypt//SECAM B/G     
	,nGermany//SECAM B/G   
	,nIraq//SECAM B        
	,nIran//SECAM B/G      
	,nHungary//SECAM D/K    
	,nLibya//SECAM B/G     
	,nLebanon//SECAM B     
	,nPoland//SECAM D/K    
	,nRomania//SECAM D/K   
	,nSaudiArabia//SECAM B/
	,nTunisia//SECAM B     
	,nRussia_d_k//SECAM D/K

	//only nmi 600 support
	,nBangladesh
	,nBrunei
	,nJordan    
	,nKuwait    
	,nOman      
	,nQatar     
	,nSriLanka  
	,nYemen    
	,nEastTimor 
	,nMaldives  
	,nMongolia 
	,nSyria    
	,nMacao    
	,nAlgeria   
	,nCameroon  
	,nGambia   
	,nGhana    
	,nKenya     
	,nLiberia   
	,nNamibia   
	,nNigeria  
	,nSierraLeone
	,nSomali   
	,nSudan     
	,nSwaziland 
	,nTanzania 
	,nUganda   
	,nZambia    

	,nAngola       
	,nZimbabwe     
	,nMali         
	,nMadagascan   
	,nSenegal      
	,nMorocco      
	,nYunisia      
	,nColombia     
	,nParaguay     
	,nPeru         
	,nBolivia      
	,nGuyana       
	,nSurinam      
	,nEcuador      
	,nCyprus       
	,nIreland      
	,nRussia_d_k_2     
	,nRussia_b_g   
	,nAustria      
	,nAzerbaijan   
	,nBelgium      
	,nCzech        
	,nDenmark      
	,nFinlang      
	,nGeorgia      
	,nGreece       
	,nIcelang      
	,nIsrael       
	,nItaly        
	,nMacedonia    
	,nMadeira      
	,nMalta        
	,nNorway       
	,nUkraine      
	,nSwitzerland  
	,nSweden       
	,nCostarica    
	,nCuba         
	,nDominica     
	,nElSalvador   
	,nGuatemala    
	,nHaiti        
	,nJamaica      
	,nPuertoRico   
	,nMontserrat   
	,nNicaragua 
	,nPanama 
	,nFiji 
	,nNewZealand
	,nSolomon
	,nUsCable   
	,nHoland   
	,nFmChina
	,nFmThai

	,nMaxRegion

#else
	nChina = 1,
	nUsa,
	nCanada,
	nKorea,
	nTaiwan,
	nMexico,
	nChile,
	nVenezuela,
	nPhilippines,
	nVietnam,
	nJapan,
	nUk,
	nHongKong,
	nSAfrica,
	nBrazil,
	nChinaSz,
	nBurma,
	nUsCable,
	nWesternEurope,
	nArgentina,
	nTurkey,
	nUAE,
	nAfghanistan,
	nSingapore,
	nThailand,
	nCambodia,
	nIndonesia,
	nMalaysia,
	nIndia,
	nPakistan,
	nPortugal,
	nSpain,
	nLaos,
	nEgypt,
	nPoland,
	nBulgaria,
	nRussia_d_k,
	nRussia_b_g,
	nIsrael,
	nHoland,
	nAzerbaijan,
	nTanzania,
	nTunisia,
	nNamibia,
	nColombia,
	nBolivia,
	nPeru,
	nBrunei,
	nKuwait,
	nOman,
	nYemen,
	nJordan,
	nCyprus,
	nGambia,
	nSierraLeone,
	nGhana,
	nNigeria,
	nCameroon,
	nSomali,
	nKenya,
	nUganda,
	nZambia,
	nSwaziland,
	nBangladesh,
	nAlgeria,
	nLiberia,
	nSriLanka,
	nQatar,
	nSudan,
	nParaguay,
	nUruguay,
	nAngola,
	nCongo,
	nMyanmar,
	nEastTimor,
	nIran,
	nMaldives,
	nMongolia,
	nSyria,
	nSaudiArabia,
	nIraq,
	nLebanon,
	nMacao,
	nLibya,
	nZimbabwe,
	nMali,
	nMadagascan,
	nSenegal,
	nMorocco,
	nYunisia,
	nGuyana,
	nSurinam,
	nEcuador,
	nIreland,
	nAustria,
	nBelgium,
	nCzech,
	nDenmark,
	nFinlang,
	nGeorgia,
	nGermany,
	nGreece,
	nHungary,
	nIcelang,
	nItaly,
	nMacedonia,
	nMadeira,
	nMalta,
	nNorway,
	nUkraine,
	nRomania,
	nSwitzerland,
	nSweden,
	nCostarica,
	nCuba,
	nDominica,
	nElSalvador,
	nGuatemala,
	nHaiti,
	nJamaica,
	nPuertoRico,
	nMontserrat,
	nNicaragua,
	nPanama,
	nAustralia,
	nFiji,
	nNewZealand,
	nSolomon,
	nFmChina,
	nFmThai
#endif	
} tNmiRegion;

typedef struct {
	tNmiRegion		region; 
	unsigned int 	schannel; 
	unsigned int	echannel;
	unsigned int	nchannel; 
} tAtvRegion;

/******************************************************************************
**
**	Nmi Atv Command Codes Defines
**		NMI_DRV_BAND_SCAN: This command code is used to perform ATV scanning.
**			Input/Output Parameters: A type of tNmiScanCallBack (function pointer) which includes
**											the following call back parameters:
**				chn (output): Channel number,
**				found (output): A flag indicate whether the current channel is valid or not,
**				last (output): A flag indicates the end of scanning.
**
*******************************************************************************/

#define NMI_DRV_BAND_SCAN					0x3

typedef int (*tNmiScanCallBack)(int chn, int found, int last);

/******************************************************************************
**
**	Nmi Atv Command Codes Defines
**		NMI_DRV_TUNE_TO_CHANNEL: This command code is used to do ATV tuning.
**			Input/Output Parameters: A type of tNmiTune which includes the following fields:
**				chn (input): Channel number,
**				poll (input): A flag indicates to start polling for the signal status (Note: the user
**										has to implement the "nmi_poll_start" function which then should
**										trigger the polling task),
**				waitforlock(input): A flag indicates whether to wait for the signal locking or not (Note:
**										this is mainly for the user who wants to use the "tune" to perform
**										the channel scanning.),
**				lock (output): A flag indicates signal is detected or not (Note: this is only meaningful when
**										the user specifies the "waitforlock" flag.).
**										  
**
*******************************************************************************/

#define NMI_DRV_TUNE_TO_CHANNEL		0x4

typedef struct {
	unsigned int chn;
	int poll;
	int waitforlock;
	int lock;
} tNmiTune;

/******************************************************************************
**
**	Nmi Atv Command Codes Defines
**		NMI_DRV_GET_STATUS: This command code is used to get the signal status.
**			Input/Output Parameters: A type of tNmiSq which includes the following fields:
**				sq (output): Signal quality; it is returned by the driver.
**
*******************************************************************************/

#define NMI_DRV_GET_STATUS			0x5

typedef struct {
	unsigned int sq;
} tNmiSq;

/******************************************************************************
**
**	Nmi Atv/FM Command Codes Defines
**		NMI_DRV_MUTE: This command code is used to mute the audio.
**			Input/Output Parameters: A type of tNmiMute which includes the following fields:
**				mute (input): Set to 1 to mute the audio, otherwise to unmute.
**
*******************************************************************************/

#define NMI_DRV_MUTE						0x6

typedef struct {
	int mute;
} tNmiMute;

/******************************************************************************
**
**	Nmi Atv Command Codes Defines
**		NMI_DRV_UV_GAIN: This command code is used to control the color.
**			Input/Output Parameters: A type of tNmiUVGain which includes the following fields:
**				uvGain (input): A range from -5 ~ 5 to control the gain of uv (Note: this is to set
**											the color to the desired level).
**											.
**
*******************************************************************************/

#define NMI_DRV_UV_GAIN					0x7

typedef struct {
	int uvGain;
} tNmiUVGain;

/******************************************************************************
**
**	Nmi FM Command Codes Defines
**		NMI_DRV_FM_SCAN: This command code is used to control the FM scan.
**			Input/Output Parameters: A type of tNmiFMScan which includes the following fields:
**				chn (input): channel number,
**				rssi (output); rssi value.
**				overth (output): over threshold counter value.
**
*******************************************************************************/

#define NMI_DRV_FM_SCAN					0x8

typedef struct {
	int chn;
	int rssi;
	int overth;
} tNmiFMScan;

/******************************************************************************
**
**	Nmi Atv Command Codes Defines
**		NMI_DRV_FM_PROCESS: This command code is used to perform the post processing of the 
**													FM scanning results. This post processing will, base on the rssi
**													value of each channel, select the best available FM channels
**													for the user.
**			Input/Output Parameters: A type of tNmiFMProcess which includes the following fields:
**				chnRssi (input): An array of rssi values from the return of the "NMI_DRV_FM_SCAN"
**											command,
**				chnRssiLen (input); the length of the "chnRssi" array,
**				chnValid (input/output): An array to held the post processing result (the FM frequency.),
**				chnValid (input): the length of the "chnValid" array,
**				chnValidCount (output): the number of the valid FM frequencies. 
**				work (input): An array for storing the temporary calculation results. It should have the
**										same length as the channel Rssi length (chnRssiLen).  
**				chnOverTh(input): An array of "over threshold counter" from the return of the "NMI_DRV_FM_SCAN"
**												command. 
**
*******************************************************************************/

#define NMI_DRV_FM_PROCESS			0x9

typedef struct {
	int *chnRssi;
	int chnRssiLen;
	int *chnValid;
	int chnValidLen;
	int chnValidCount;
	int *work;
	int *chnOverTh;
} tNmiFMProcess;

/******************************************************************************
**
**	Nmi Atv Command Codes Defines
**		NMI_DRV_UV_SNR_THRESHOLD: This command code is used to set the Snr threshold for
**			uv tracking.
**			Input Parameters: A type of tNmiUVSnrTh which includes the following fields:
**				snrHigh (input): the Snr upper bound. The UV gain tracking will start once the Snr is below
**					this value,
**				snrLow (input): the Snr lower bound. The UV gain will set to 0 (black and white) once
**					the Snr is below this value.
**
*******************************************************************************/

#define NMI_DRV_UV_SNR_THRESHOLD	0xa

typedef struct {
	double snrHigh;
	double snrLow;
} tNmiUVSnrTh;

/******************************************************************************
**
**	Nmi FM Command Codes Defines
**		NMI_DRV_FM_SEEK: This command code is used to control the seeking of the FM channel.
**			Input/Output Parameters: A type of tNmiFMSeek which includes the following fields:
**				firstchannel (input): A flag indicates the starting of the FM seek (Note: the user should
**													clear this flag in the subsequent calls),
**				chn (input): channel number,
**				found (output): A flag indicates the current channel has valid FM signal.
**
*******************************************************************************/

#define NMI_DRV_FM_SEEK					0xb

typedef struct {
	int firstchannel;
	int chn;
	int found;
	int rssi;
} tNmiFMSeek;

/******************************************************************************
**
**	Nmi Atv/FM Command Codes Defines
**		NMI_DRV_SET_VOLUME: This command code is used to set the volume.
**			Input Parameters: A type of tNmiVolume which includes the following fields:
**				step (input): the volume step from -3 ~ +3.
**
*******************************************************************************/

#define NMI_DRV_SET_VOLUME	0xc

typedef struct {
	int step;
} tNmiVolume;

/******************************************************************************
**
**	Nmi FM Command Codes Defines
**		NMI_DRV_FM_SEEK_SELECT: This command code is used to decide whether the current
**			channel is a valid FM signal based on the neighbor's rssi values.
**			Input/Output Parameters: A type of tNmiFMSeekSelect which includes the following fields:
**				rssi (input): An array which contains 5/7 rssi values. (Note: The rssi values in the 
**					array should arrange in the following manner:
**							0					1				2				3						4
**					channel-2 	channel-1	channel	channel+1	channel+2
**				overth (input): An array which contains 5/7 "over threshold counter" values. (Note: This
**					array should have the same order as rssi array.).
**				found (output): A flag indicates the current channel has valid FM signal.
**
*******************************************************************************/

#define NMI_DRV_FM_SEEK_SELECT	0xd

typedef struct {
	int rssi[7];
	int overth[7];
	int sz;
	int found;
} tNmiFMSeekSelect;

/******************************************************************************
**
**	Nmi FM Command Codes Defines
**		NMI_DRV_FM_DIRECT_TUNE: This command code is used to directly tune to the FM channel.
**			Input/Output Parameters: A type of tNmiFMTune which includes the following fields:
**				frequency (input): FM frequency in Hz,
**				rssi (output): adjust RSSI value for the current frequency.
**
*******************************************************************************/

#define NMI_DRV_FM_DIRECT_TUNE	0xe

typedef struct {
	unsigned int frequency;
	int rssi;
	int overth;
} tNmiFMTune;

/******************************************************************************
**
**	Nmi FM Command Codes Defines
**		NMI_DRV_FORCE_COLOR: This command code is used to enable the color.
**			Input Parameters: A type of tNmiForceColor which includes the following fields:
**				force (input): Set to 1 to enable color; otherwise disable the color (black and white),
**
*******************************************************************************/

#define NMI_DRV_FORCE_COLOR 0xf

typedef struct {
	int force;
} tNmiForceColor	;

/******************************************************************************
**
**	Nmi FM Command Codes Defines
**		NMI_DRV_SHOW_PATTERN: This command code is used to enable the test pattern.
**			Input Parameters: A type of tNmiPattern which includes the following fields:
**				enable (input): Set to 1 to enable the pattern display; otherwise stop the pattern
**					display (Note: Before using this function, the "enable_pattern" parameter in the
**					"tAtvInit" structure has to be set to 1). 
**
*******************************************************************************/

#define NMI_DRV_SHOW_PATTERN 0x10

typedef struct {
	int enable;
} tNmiPattern;															

/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_SET_SCAN_LEVEL: This command code is used to set the scan threshold.
**			Input Parameters: A pointer to an interger type,
**				level (input): 1 to 7 is the valid range with the 7 is the most relax threshold and
**					1 is the most strigent (the default is 5).
**
*******************************************************************************/

#define NMI_DRV_SET_SCAN_LEVEL 0x11

/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_SET_UV_IIR_FACTOR: This command code is used to control the UV adjustment
**			speed.
**			Input Parameters: A pointer to a double type,
**				factor (input): A value from 0 ~ 1 with 0 means no change (fixed gain) and 1 has 
**					fastest chnage of color.
**
*******************************************************************************/

#define NMI_DRV_SET_UV_IIR_FACTOR 0x12

/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_SET_AUDIO_FILTERS: This command code is used to set the audio filters.
**			Input Parameters: A type of tNmiAudioFilters which includes the following fields
**				filter7 (input): A value from 0 ~ 7,
**				filter6 (input): A value from 0 ~ 7,
**				filter5 (input): A value from 0 ~ 7,
**				filter4(input): A value from 0 ~ 7,
**				filter3 (input): A value from 0 ~ 7,
**				filter2 (input): A value from 0 ~ 7,
**				filter1 (input): A value from 0 ~ 7,
**				filter0 (input): A value from 0 ~ 7.
**
*******************************************************************************/

#define NMI_DRV_SET_AUDIO_FILTERS 0x13
typedef struct {
	int filter7;
	int filter6;
	int filter5;
	int filter4;
	int filter3;
	int filter2;
	int filter1;
	int filter0;
} tNmiAudioFilters;

/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_SET_OUTPUT_SIZE: This command code is used to set the ATV output size.
**			Input Parameters: A type of tBtSize which specify the desired output size.
**
*******************************************************************************/

#define NMI_DRV_SET_OUTPUT_SIZE 0x14
#if 0
typedef enum {
	bt320x240,
	bt80x60,
	bt176x144,
	bt220x176,
	bt240x180,
	bt426x320,
	bt360x240,
	bt480x280,
	bt480x360,
} tBtSize;
#endif
/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_SET_GAMMA: This command code is used to set the ATV gamma coefficient.
**			Input Parameters: An integer value from 0 to 50.
**
*******************************************************************************/
#define NMI_DRV_SET_GAMMA 0x15

/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_GET_RSSI: This command code is used to retrieve the current RSSI value.
**			Input/Output Parameters: A pointer to an integer type:
**				rssi (output): A negative Rssi value.  
**
*******************************************************************************/
#define NMI_DRV_GET_RSSI 0x16

/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_GET_VSYNC_LOCK: This command code is used to retrieve the current VSYNC
**			lock status.
**			Input/Output Parameters: A pointer to an integer value.
**				lock (output): A value of 1 means the VSYNC is locked; otherwise the VSYNC is not
**					locked.
**
*******************************************************************************/
#define NMI_DRV_GET_VSYNC_LOCK 0x17
/******************************************************************************
**
**	Nmi ATV Command Codes Defines
**		NMI_DRV_GET_CN: This command code is used to retrieve the current C/N value
**			Input Parameters: An pointer to an double type.
**				cn (output): The current C/N value.
**
*******************************************************************************/
#define NMI_DRV_GET_CN 0x18

/******************************************************************************
**
**	Nmi Atv Command Codes Defines
**		NMI_DRV_VERSION: This command code is used to retrieve the version of driver.
**			Input/Output Parameters: A type of tNmiDriverVer which includes the following fields:
**				major (output): Driver major version number,
**				minor (output): Driver minor version number,
**				rev1 (output): revision number,
**				rev2 (output): sub-revision number.
**
*******************************************************************************/

#define NMI_DRV_VERSION								0x20000000
typedef struct
{
	int major;
	int minor;
	int rev1;
	int rev2;
} tNmiDriverVer;

/******************************************************************************
**
**	Nmi Atv Exported Function Declaration 
**		A global entry function for the Nmi driver.
**			Input/Output Parameters:
**				code (input only): A command code defines by the above,
**				inp (input/output): A void pointer which contains the related data structure address
**					for each command code.
**
*******************************************************************************/

tNmiRetCode nmi_drv_ctl(unsigned long code, void *inp);

/////////////////////////aux function////////////////////////////////////////////////////
//for nmi600
void nmi_atv_set_frame_triger_thrd(unsigned int threshold);

int nmi_get_i2c_err_state(void);

//level: 0-7  default:0
void nmi_atv_set_driving_ability(unsigned int level);

//only for nmi600
void nmi_atv_set_comb_filter(void);

//just call it after init
void nmi_atv_set_videoavg(void);

//only for nmi601
void nmi_atv_set_fieldoutput(unsigned int fieldmap);

void nmi_atv_set_spur_table(void);

void nmi_atv_set_luma_gain(unsigned char gain);

void nmi_atv_set_luma_offset(int offset);

unsigned int nmi_atv_get_color_lock(void);

#endif
