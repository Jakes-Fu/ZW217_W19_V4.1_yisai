/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmiatv.h
**		These are the parameters used by the ASIC driver. 
**
** 
*******************************************************************************/
#ifndef _NMIATV_H_
#define _NMIATV_H_

/******************************************************************************
**
**	NMI Atv Interface Functions Parameters
**
*******************************************************************************/

typedef struct {
	uint32_t frequency;				/* in Hz */
	tNmiTvStandard standard;
} tAtvTune;

typedef struct {
	int hcorr;
	int vcorr;
	int vsynclock;
	int colorlocklevel;
} tAtvVdecSyncStatus;

typedef struct {
	int stereo;
	int filter;
	int state;
	int levellow;
	int levelhigh;
	double snr;
	int plllevel;
} tAtvAdecAqmStatus;

typedef struct {
	double 	lnadb;
	uint32_t  lna;
	double		bblidb;
	uint32_t	bbli;
} tAtvRFGain;

typedef struct {
	int 						chn;
	uint32_t 				frequency;
	tNmiTvStandard standard;	
} tNmiAtvChannel;

typedef struct {
	uint8_t filter7;
	uint8_t filter6;
	uint8_t filter5;
	uint8_t filter4;
	uint8_t filter3;
	uint8_t filter2;
	uint8_t filter1;
	uint8_t filter0;
} tNmiAudFilter;

typedef struct {
	int of;
	uint32_t cf;
	uint32_t util;
} tAtvFBStatus;

typedef enum{
   sw_256KHz=0,  //+/- 128Khz
   sw_128KHz=1,  //+/- 64 Khz
   sw_64KHz =2,  //+/- 32 Khz
   sw_32KHz =3,  //+/- 16 Khz
   sw_16KHz =4,  //+/- 8  Khz
   sw_8KHz  =5,  //+/- 4  Khz
   sw_4KHz  =6,  //+/- 2  Khz
   sw_2KHz  =7   //+/- 1  Khz
}spurwidth;


typedef struct {
	uint32_t sfrequency;
	int width;
	int local;
} tNotchPara;

/******************************************************************************
**
**	NMI Atv Function Table
**
*******************************************************************************/

typedef struct {
	int (*init)(void);
	uint32_t (*getchipid)(void);
	int (*scan)(tAtvTune *);
	uint32_t (*r32)(uint32_t);
	void (*w32)(uint32_t, uint32_t);
	uint8_t (*rfr)(uint32_t);
	void (*rfw)(uint32_t, uint8_t);
	void (*tune)(tAtvTune *);
 	int (*checklock)(uint32_t);
	void (*reset)(void);
	double (*getfreqoffset)(void);
	double (*gettimeoffset)(void);
	double (*getfedagc)(void);
	int (*getfeagclock)(void);
	int (*getfecplllock)(void);
	void (*getvdecsync)(tAtvVdecSyncStatus *);
	int (*getvdecblolock)(void);
	uint32_t (*getvdeclinewidth)(void);
	double (*getvdecnoise)(void);
	void (*trackuvgain)(void);
	void (*setuvgain)(int);
	void (*setuvsnrth)(double, double);
	uint32_t (*getvdecsnr)(void);
	int (*getadecagclock)(void);
	double (*getadecagcgain)(void);	
	int (*getadeclock)(void);
	void (*getadecstatus)(tAtvAdecAqmStatus *);
	void (*play)(void);
	void (*mute)(int);
	int (*getrssi)(void);
	int (*getoverthcount)(void);
	void (*setvolume)(int);
	void (*forcecolor)(int);
	void (*resetstart)(tNmiTvStandard);
	double (*getspur)(void);
	void (*showpattern)(int);
	void (*applyoffset)(double);
	void (*fmavgrssi)(int *, int, int*,int *);
	void (*fmscanlist)(int *, int *, int *, int *, int, int, int *);
	int (*fmseek)(int *,int *, int);
	void (*setscanlevel)(int);
	void (*setuviir)(double);
	void (*setaudiofilter)(tNmiAudFilter *);
	void (*sleep)(int);
	void (*notchctl)(int, uint32_t, int);
	void (*notchchk)(void);
	void (*getrfgain)(tAtvRFGain *);
	void (*setsize)(tNmiOutputSize);
	int (*getferssi)(void);
	void (*getfbstatus)(tAtvFBStatus *);
	void (*setlumagain)(int);
	int (*getplllevel)(void);
	void (*setfastscan)(int);
	void (*setminscale)(double);
	double (*getnotchfreq)(void);
	void (*setfieldoutput)(uint32_t, tNmiTvStandard);
	void (*uselocalpeak)(int);
	void (*setnotch)(tNotchPara *, int);
	void (*setgamma)(int);
	void (*setlumaoffset)(int);
	void (*trackdoppler)(void);
	void (*setfmscanth)(int);
	void (*setchswitchspeed)(int);
	void (*setlinedelay)(int);
	void (*acictl)(int);
	void (*setscansnrlevel)(double);
	int (*getbuserror)(void);
} tNmiAtvVtbl;

/******************************************************************************
**
**	NMI Atv OS dependent Functions
**
*******************************************************************************/

typedef struct {
	int (*read)(uint8_t, uint8_t *, uint32_t);
	int (*write)(uint8_t, uint8_t *, uint32_t);
	void (*delay)(uint32_t);
	uint32_t (*gettick)(void);
	void (*log)(char *);
} tNmiHlpVtbl;

/******************************************************************************
**
**	NMI Atv Common Share Data Structure
**
*******************************************************************************/

typedef struct {
	uint32_t					xo;
	tNmiBtFormat			btformat;
	tNmiBt656Clk			btrate;
	uint32_t					zone;
	tNmiHlpVtbl				hlp;
	tNmiI2SFormat		i2s;
	tNmiI2SRate 			i2s_rate;
	int clk_polarity;
	int vsync_polarity;
	int hsync_polarity;
	int input_order;
	int blank_interval;
	int interlace;
	int enablepattern;
	int mode;
	tNmiOutputSize size;
	int uvstep;
	int vbl;						/* BT656 only, not 601 */
	int half_frame_rate;	/* 601 only */
	tNmiIOVoltage io;
} tNmiIn;

#endif
