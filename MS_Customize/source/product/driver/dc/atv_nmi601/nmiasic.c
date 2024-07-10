/******************************************************************************
**
**	Copyright (c) Newport Media Inc.  All rights reserved.
**
** 	Module Name:  nmiasic.c
**		This module includes the Asic C file into the driver build.
**
**
**   PLEASE DO NOT MODIFY THIS FILE	!!!
** 
*******************************************************************************/
/*lint -save -e737 -e533 -e771 -e774 -e514 -e644 -e661 -e662 -e718 -e18 -e777 -e618 -e746 -e648 -e790 -e502*/	
#include "nmiport.h"

//#include "nmicmn.c"
#if 0
#include "nmicmn.c"
#else

#include "nmicmn.h"

typedef struct {
	uint32_t				chipid;
	uint32_t				frequency;
	uint32_t				lofrequency;
	tNmiTvStandard	standard;
	double					plloffset;
	uint32_t				u_gain;
	uint32_t				v_gain;
	double					uvSnrH;
	double					uvSnrL;
	uint32_t				uscale;
	uint32_t				vscale;
	int						vscaleSecam;
	double					uvfact;
	int						notchOn;
	uint32_t				i2s;
	uint32_t 				adec;
	int						pattern;
	int						alocklevelth; 
	int						linewidthdevth;
	int						apply;
	double					applyoffset;
	uint32_t				audfilter;

	int						hsyncCount;
	int						hsyncMode;
	double					nctrssi;

	uint8_t					vol;
	uint8_t					volfm;
	uint8_t					volntsc;
	uint8_t					volpal;

	int						fastscan;
	double					minScale;

	int						notch_apply;
	uint32_t				notch_fq;
	int						notch_bw;
	int						notch_dac;
	int						notch_rssi;

	double					digiclk;
	int						doscan;
	int						localpeak;

	int						lumaOff;

	double 					bloAvg;
	int						i_nv;
	double					decreAvg;
	int 					decreDiff;

	int 					fmscanth;

	int					pkg4x4;

	double				snrth;
} tNmiPriv;

typedef struct {
	tNmiIn				inp;
	tNmiPriv			priv;		
	int					buserror;
} tNmiAsic;

static tNmiAsic chip;


static void nmi_debug(uint32_t zone, char *fmt,...)
{
	char buf[256];
	va_list args;
	int len;

	if (zone & chip.inp.zone) {
	    //lint -esym(628, __va_*)
		va_start(args, fmt);/*lint !e628*/

		strcpy(buf, "[Nmi Asic]: "); 
		len = vsprintf(&buf[12], fmt, args);
		va_end(args);

		if (chip.inp.hlp.log) {
			chip.inp.hlp.log(buf);
		}
	}

	return;
}

static void nmi_delay(uint32_t msec)
{
	if (chip.inp.hlp.delay) {
		chip.inp.hlp.delay(msec);
	}
}

static uint32_t nmi_get_tick(void)
{
	uint32_t tick = 0;

	if (chip.inp.hlp.gettick) {
		tick = chip.inp.hlp.gettick();
	}

	return tick;
}


static uint32_t rReg32(uint32_t adr)
{
	uint8_t b[16];
	uint8_t r[4]={0};
	uint32_t val = 0;
	int len, rsz;

	if (!(chip.inp.hlp.write && chip.inp.hlp.read) ||
			chip.buserror) {
		nmi_debug(N_ERR, "Error, read register(32), can't access chipset\n");
		return 0;
	}		 

	if ((adr >= 0x21900) && (adr <= 0x21906)) {
		b[0] = 0x28;
		b[1] = (uint8_t)(adr >> 24);
		b[2] = (uint8_t)(adr >> 16);
		b[3] = (uint8_t)(adr >> 8);
		b[4] = (uint8_t)(adr);
		b[5] = 0x01;
		rsz = 1;
	} else {
		b[0] = 0x80;										
		b[1] = (uint8_t)(adr >> 24);
		b[2] = (uint8_t)(adr >> 16);
		b[3] = (uint8_t)(adr >> 8);
		b[4] = (uint8_t)(adr);
		b[5] = 0x04;
		rsz = 4;
	}
	len = 6;

	if (chip.inp.hlp.write(0x60, b, len) < 0) {
		nmi_debug(N_ERR, "Failed, bus write...\n");
		chip.buserror = 1;
		return 0;
	}

	if (chip.inp.hlp.read(0x60, (uint8_t *)r, rsz) < 0) {
		nmi_debug(N_ERR, "Failed, bus read...\n");
		chip.buserror = 1;
	}
	
	val =  (uint32_t)r[3] << 24 |(uint32_t)r[2] << 16 | (uint32_t)r[1] << 8 |r[0] ; 

	return val; 
}

static void wReg32(uint32_t adr, uint32_t val)
{
	uint8_t b[16];
	int len;

	if (!(chip.inp.hlp.write && chip.inp.hlp.read) ||
			chip.buserror) {
		nmi_debug(N_ERR, "Error, write register(32), can't access chipset\n");
		return;
	}		 

	if ((adr >= 0x21900) && (adr <= 0x21906)) {	
		b[0] = 0x38;
		b[1] = (uint8_t)(adr >> 24);
		b[2] = (uint8_t)(adr >> 16);
		b[3] = (uint8_t)(adr >> 8);
		b[4] = (uint8_t)adr;
		b[5] = 0x01;
		b[6] = (uint8_t)val;
		len = 7;
	} else {
		b[0] = 0x90;
		b[1] = (uint8_t)(adr >> 24);
		b[2] = (uint8_t)(adr >> 16);
		b[3] = (uint8_t)(adr >> 8);
		b[4] = (uint8_t)adr;
		b[5] = 0x04;
		b[6] = (uint8_t)val;
		b[7] = (uint8_t)(val >> 8);
		b[8] = (uint8_t)(val >> 16);
		b[9] = (uint8_t)(val >> 24);
		len = 10;
	}

	if (chip.inp.hlp.write(0x60, b, len) < 0) {
		nmi_debug(N_ERR, "Failed, bus write...\n");
		chip.buserror = 1;
	}

	return; 
}

static void wRegBurst(uint32_t adr, uint8_t *bd, int sz)
{
	uint8_t b[256];
	int len, i;
	uint32_t val = 0;

	if (sz > (256-6)) {
		nmi_debug(N_ERR, "Failed, bus burst write, size too big...(%d)\n", sz);
		return; 
	}

	b[0] = 0x90;
	b[1] = (uint8_t)(adr >> 24);
	b[2] = (uint8_t)(adr >> 16);
	b[3] = (uint8_t)(adr >> 8);
	b[4] = (uint8_t)adr;
	b[5] = sz;
	
	for (i = 0; i < sz/4; i++) {
		val = *((uint32_t *)(bd + 4*i));
		b[6+4*i] = (uint8_t)(val);	
		b[6+4*i+1] = (uint8_t)(val >> 8);
		b[6+4*i+2] = (uint8_t)(val >> 16);
		b[6+4*i+3] = (uint8_t)(val>> 24);
	}

	len = sz + 6;

	if (chip.inp.hlp.write(0x60, b, len) < 0) {
		nmi_debug(N_ERR, "Failed, bus burst write...\n");
		chip.buserror = 1;
	}

	return;
}


//#include "nmiatv.c"
#if 0
#include "nmiatv.c"
#else
#define LUMA_OFFSET_STEP 128
#define SYNCMODE 

static void atv_demod_reset_start(tNmiTvStandard);
static void atv_demod_reset(void);
static int atv_demod_check_lock(uint32_t);
static int atv_fe_lock(void);
static int atv_vdec_blo_lock(void);
static void atv_toggle_gpio(uint32_t);
static void atv_fe_config(tNmiTvStandard, double);
static void atv_vdec_config(tNmiTvStandard);
static void atv_be_config(tNmiTvStandard);
static void atv_adec_config(tNmiTvStandard, double);
static void atv_be_set_mode(uint32_t); 
static double atv_demod_freq_offset(void);
static double atv_demod_time_offset(void);
static int atv_get_rssi(void);
static int atv_fe_get_rssi(void);
static int atv_fe_agc_lock(void);
static void atv_vdec_set_hsync_mode(int);
static void atv_fe_use_local_agc(tNmiTvStandard);
static void atv_fpga_play(void);
static void atv_vdec_doppler_enhancement(void);
static void atv_vdec_set_uv_gain(int step);

#define NUM_AVSEP_COEFF_REGS	48

const static uint32_t atv_av_sep_coefficents_vbw_4o2mhz_m_n_ntsc_fm[3][48] = 
{
  {0x003d0006,0x00230031, 0xffeb0007, 0xfff8ffe6, 0x001d0012, 0xfff1000e, 
	0xffe9ffde, 0x00240009, 0xffff0021, 0xffd5ffdb, 0x0022fff5, 0x00190035, 
	0xffc4ffe5, 0x0010ffd6, 0x003b0040, 0xffc00000, 0xffebffb4, 0x005b003a, 
	0xffd3002d, 0xffb8ff9a, 0x006c0019, 0x00020064, 0xff81ff96, 0x005fffdc, 
	0x004b0097, 0xff58ffb6, 0x0029ff89, 0x00a500b1, 0xff510003, 0xffc5ff2e, 
	0x00fb009e, 0xff84007e, 0xff37fee4, 0x01300046, 0x0005011c, 0xfe8dfece, 
	0x011eff98, 0x00e701cb, 0xfde0ff15, 0x0090fe78, 0x02550270, 0xfd490006, 
	0xff05fc99, 0x050102f2, 0xfce202b4, 0xf93ff7f2, 0x13070339, 0x2769218a},
  {0x00070028, 0xffddfff3, 0xffe7ffd7, 0x001e0006, 0x0007001f, 0xffe1ffeb, 
	0x0012fff3, 0x00150023, 0xffd8fff3, 0x0002ffdd, 0x00300029, 0xffdd000e, 
	0xffdfffc5, 0x00400017, 0xfffa0034, 0xffb9ffc0, 0x0039fff0, 0x002b0057, 
	0xff9effd6, 0x0011ffb7, 0x00660064, 0xffa50010, 0xffc9ff81, 0x00910047, 
	0xffdb0063, 0xff71ff68, 0x0090fff7, 0x004300b8, 0xff29ff8a, 0x0049ff7c, 
	0x00c900e7, 0xff1cfff8, 0xffb3fef4, 0x014600c7, 0xff7300b2, 0xfedbfe91, 
	0x017f002f, 0x005401a0, 0xfde2fe95, 0x0125fefb, 0x01ec0298, 0xfcf9ff64, 
	0xffa7fcdf, 0x04e40366, 0xfc53021e, 0xf9bff7e4, 0x134003da, 0x26c2212a},
  {0xffc6fffe, 0xffd9ffd0, 0x0010fff3, 0x000f001b, 0xffe3fff5, 0x0003ffe9, 
	0x0021001d, 0xffe80009, 0xffe8ffd8, 0x002d000f, 0x00000027, 0xffcbffd5, 
	0x0022ffec, 0x002a003f, 0xffbeffef, 0xfff8ffc0, 0x0053003c, 0xffd50025, 
	0xffbaffa2, 0x005f0010, 0x00160064, 0xff84ffaf, 0x0035ffbe, 0x00700089, 
	0xff7cfff7, 0xffd1ff67, 0x00b8006c, 0xffc2006e, 0xff51ff3c, 0x00b8fffb, 
	0x005900e9, 0xfeeeff71, 0x0046ff48, 0x0119011f, 0xfef70023, 0xff58fe8f, 
	0x01b100c5, 0xffb40144, 0xfe13fe36, 0x01abff96, 0x01660299, 0xfcc4fec5, 
	0x004ffd41, 0x04b003ca, 0xfbc8017c, 0xfa49f7e4, 0x1375047d, 0x261620c5}
};	

const static uint32_t atv_av_sep_coefficents_vbw_5o5mhz_i[3][48] = {
{0x00350009, 0x00160033, 0xffe0ffea, 0x0019fffd, 0xffed000d, 0x0009ffe9, 
0x0007001f, 0xffe8ffe2, 0x00250013, 0xffd7ffff, 0x0021ffea, 0xfff3002b, 
0xfff0ffca, 0x002d0032, 0xffbfffe2, 0x0046fffd, 0xffcb002a, 0x0010ffb5, 
0x0020005a, 0xffb1ffb0, 0x006d002c, 0xff90000c, 0x0050ffb5, 0xffed007d, 
0xffc5ff6f, 0x0084007d, 0xff4effc1, 0x00b1ffe4, 0xff85007f, 0x0015ff33, 
0x006900ea, 0xff22ff3a, 0x0126005e, 0xfede003a, 0x00c5ff21, 0xffe90161, 
0xff3bfe6d, 0x01980154, 0xfddeff64, 0x0227ff80, 0xfe8401c7, 0x0018fd17, 
0x01eb0386, 0xfbb0fcc7, 0x06bf0196, 0xf7280202, 0x0a43f67b, 0x353d2753},
{0xfff30011, 0xffd6ffda, 0x001cfff5, 0xffff0020, 0xfff0ffe3, 0x00180012, 
0xffe2fff9, 0x001dfff7, 0xfff0001b, 0xfff9ffd8, 0x00200028, 0xffcfffe7, 
0x0034fffe, 0xffdb0022, 0x0006ffc5, 0x00220044, 0xffbaffca, 0x00560011, 
0xffb60020, 0x0022ffb1, 0x001a006a, 0xffa9ff9c, 0x00800039, 0xff7e000e, 
0x0057ffa4, 0xfffb0096, 0xffa4ff5a, 0x00ae007d, 0xff30ffde, 0x00afffaa, 
0xffb600c5, 0xffb8fefd, 0x00dc00f0, 0xfec1ff7c, 0x0144ffd4, 0xff2900f0, 
0x0004fe76, 0x010301b9, 0xfe11feac, 0x02680057, 0xfdde0112, 0x00f2fd73, 
0x011d03a0, 0xfc34fc3d, 0x06ad0269, 0xf6c10124, 0x0b05f722, 0x345a2715},
{0xffd3ffd9, 0x0010ffe7, 0x00100024, 0xffe6ffed, 0x001a0002, 0xffeb000c, 
0x0008ffe7, 0x000d0022, 0xffdeffe3, 0x002b000a, 0xffdc0011, 0x000dffd6, 
0x00140036, 0xffccffd1, 0x00440012, 0xffc50018, 0x0017ffc0, 0x001c0054, 
0xffb3ffb7, 0x0067001e, 0xffa50021, 0x0027ffa3, 0x0025007e, 0xff90ff8f, 
0x00990033, 0xff740029, 0x0043ff7b, 0x002e00bb, 0xff61ff53, 0x00e40057, 
0xff280032, 0x0072ff41, 0x00350119, 0xff18feef, 0x015f0098, 0xfea10039, 
0x00cefee1, 0x003b01c3, 0xfe90fe2c, 0x02600123, 0xfd69003e, 0x01befe05, 
0x003f0384, 0xfcd8fbd7, 0x067e0330, 0xf66c0040, 0x0bc3f7d4, 0x337426d2}
};	

const static uint32_t atv_av_sep_coefficents_vbw_5o0mhz_b_b1_g_h_d_d1_k_k1_l[3][48] = {
{0xffe5ffe0, 0x001afffa, 0x000e0025, 0xffe1ffeb, 0x0018fffa, 0xfff80017, 
0xfff4ffe3, 0x001f0017, 0xffdbfffd, 0x0015ffe6, 0x000d002d, 0xffd3ffd9, 
0x00370007, 0xffdf0023, 0xfff3ffc1, 0x003b003b, 0xffb1ffed, 0x0037ffd7, 
0x00080057, 0xffb3ffa8, 0x006f0024, 0xffab002e, 0x0003ff8d, 0x005e007f, 
0xff68ffbf, 0x0081ffd2, 0xffe70096, 0xff90ff4b, 0x00ce006e, 0xff3e0027, 
0x0040ff41, 0x00810101, 0xfee8ff4a, 0x0125ffee, 0xff7c00f6, 0xff71fe8b, 
0x0189012d, 0xfe32ffe2, 0x0108feb9, 0x009a0242, 0xfda1fde3, 0x03400093, 
0xfda501f1, 0xff5ffbae, 0x0533050f, 0xf5ddfd7e, 0x0df0fa3a, 0x30a325e0},
{0x00320008, 0x00220033, 0xffe2fffc, 0x000affeb, 0x00070019, 0xffe7ffea, 
0x001f0003, 0xfff00016, 0xfff4ffdc, 0x0026001e, 0xffd4fffe, 0x0015ffde, 
0x00150036, 0xffc7ffd7, 0x003dffff, 0xffe70033, 0xffddffb4, 0x00520037, 
0xffac0007, 0x001effb4, 0x0036006a, 0xff8cffb9, 0x0070ffef, 0xffde006d, 
0xffafff6f, 0x00a1005c, 0xff6a001f, 0x0026ff66, 0x007700c6, 0xff1fff89, 
0x00caffcb, 0xffd700dd, 0xff4ffeee, 0x0140009c, 0xfee80059, 0x002cfebb, 
0x0112018b, 0xfe1dff28, 0x01a1ff66, 0xffd20207, 0xfe25fd87, 0x034e0154, 
0xfd0b0137, 0x002ffbf4, 0x04a30566, 0xf5d8fcb9, 0x0e87fafc, 0x2fd02590},
{0xfffc0013, 0xffd5ffe4, 0x0004ffe1, 0x001b0020, 0xffe4fffc, 0x000dffee, 
0x0008001c, 0xffe3ffe7, 0x00240004, 0xffef001b, 0xffebffd3, 0x0031001f, 
0xffd10008, 0x000affcf, 0x002a003d, 0xffb7ffe0, 0x0038ffe5, 0x0005004d, 
0xffb7ffb0, 0x00640018, 0xffc5003b, 0xffe0ff8e, 0x00740060, 0xff7dfffa, 
0x0035ff98, 0x004c009e, 0xff53ff95, 0x00a2ffe2, 0xffe000a9, 0xff71ff2d, 
0x00f7006c, 0xff41005c, 0xfff2fef9, 0x00f90114, 0xfea1ffa6, 0x00d9ff39, 
0x00690196, 0xfe54fe97, 0x02020026, 0xff120192, 0xfeccfd65, 0x032a01fc, 
0xfc920072, 0x00fbfc5a, 0x040505a1, 0xf5e4fc01, 0x0f15fbbe, 0x2f01253e}
};	

#define	NUM_SUPPORTED_STANDARDS		12	

typedef struct {
	uint32_t color_standard;				
	uint32_t blo_fsc_seed;				
	uint32_t first_active_line;			
	uint32_t last_active_line;			
	uint32_t hsync_to_burst_start;		
	uint32_t hsync_to_burst_end;			
	uint32_t active_video_start;			
	uint32_t active_video_end	;
			
	uint32_t backporch_value;				
	uint32_t mun_serr_lines;					
	uint32_t serr_line_index;				
	uint32_t line_duration;					
	uint32_t inv_linewidth;					
	uint32_t num_vsync_pulses;
	uint32_t num_lines_per_frame;		
	uint32_t num_lines_per_field;		

	uint32_t tx_sig_mean;						
	uint32_t useful_duration;				
	uint32_t active_vid_hcnt_max;		
	uint32_t active_vid_vcnt_max;		
	uint32_t remod_blank_level;			
	uint32_t frontend_standard;			
	uint32_t aci_mode;								
	uint32_t frontend_ddfs;
					
	uint32_t frontend_ddfs_601b0;
	uint32_t repeater_enable;				
	uint32_t hscale_enable;					
	uint32_t vscale_enable;					
	uint32_t hcnt_start	;							
	uint32_t hcnt_end;		
	uint32_t vcnt_start;							
	uint32_t vcnt_end;
								
	uint32_t rptr_right;							
	uint32_t rptr_left;							
	uint32_t rptr_evn_top;						
	uint32_t rptr_evn_bottom;				
	uint32_t rptr_odd_top;						
	uint32_t rptr_odd_bottom;				
	uint32_t rptr_blank;							
	uint32_t emphasis_filter;

	uint32_t	hcnt_start_601;
	uint32_t  rptr_right_601;

	uint32_t first_active_line_n;
	uint32_t last_active_line_n;
	
	uint32_t odd_field_offset;
	uint32_t odd_field_f1;
	uint32_t odd_field_l1;
	uint32_t arej;
	
	uint32_t serr_line_end1;
	uint32_t serr_line_start2;
	uint32_t serr_line_end2; 
} tAtvStandardPara;

const static tAtvStandardPara atv_standard_params[NUM_SUPPORTED_STANDARDS] = { 
{0, 0x43e0f7ad, 20, 260, 71, 105, 130, 840, 
						130, 21, 0, 858, 19553, 6, 525, 263, 
						1328, 728, 709, 239, 881, 0, 0, 0x96c1, 
						0x13e94, 0, 1, 0, 3, 706, 0, 239, 
						0, 0, 0, 0, 0, 0, 269, 0,
						3, 0,
						16, 256,
						1, 1, 0, 0,
						18, 260, 263},					
{1, 0x541315e5, 24, 311, 75, 105, 142, 844,
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a, 
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313},					
{1, 0x541315e5, 24, 311, 75, 105, 142, 844,
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a, 
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,			
						1, 0, 1, 1,
						23, 310, 313},					
{1, 0x43cdd101, 20, 260, 71, 105, 130, 840, 
						130, 21, 0, 858, 19553, 6, 525, 263, 
						1328, 728, 709, 239, 881, 0, 0, 0x96c1, 
						0x13e94, 0, 1, 0, 3, 706, 0, 239, 
						0, 0, 0, 0, 0, 0, 269, 0,
						3, 0,
						16, 256,
						0, 0, 1, 0,
						18, 260, 263},
{1, 0x43ed288d, 24, 311, 76, 106, 130, 840, 
						130, 26, 1, 864, 19418, 6, 625, 313, 
						1328, 728, 709, 286, 881, 0, 0, 0x96c1, 
						0x13e94, 0, 1, 1, 3, 706, 0, 286, 
						0, 0, 0, 1, 0, 1, 269, 0,
						3, 0,
						20, 307,
						0, 1, 0, 0,
						23, 310, 313},
{1, 0x541315e5, 24, 311, 75, 105, 142, 844, 
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a, 
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313},
{1, 0x541315e5, 24, 311, 75, 105, 142, 844, 
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a, 
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313},
{1, 0x541315e5, 24, 311, 75, 105, 142, 844, 
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a, 	
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313},					
{1, 0x541315e5, 24, 311, 75, 105, 142, 844, 
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a,
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313},
{1, 0x541315e5, 24, 311, 75, 105, 142, 844, 
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 1, 0xbc4d, 
						0x18a6e, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313},
{1, 0x541315e5, 24, 311, 75, 105, 142, 844, 
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a,
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313},
{1, 0x541315e5, 24, 311, 75, 105, 142, 844, 
						142, 26, 1, 864, 19418, 5, 625, 313, 
						1328, 720, 701, 286, 881, 1, 2, 0xe23a, 
						0x1d648, 1, 1, 1, 0, 701, 0, 286, 
						1, 1, 0, 1, 0, 1, 269, 1,
						1, 2,
						20, 307,
						0, 1, 0, 1,
						23, 310, 313}	
};

const static uint32_t serration_line_indeces[2][27]	= {

{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 260, 261, 262, 0,	
0, 0, 0, 0, 0},	
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
22, 310, 311, 312, 0}
};	
					
const static uint32_t adec_lpf_coeff[4][56] = {
{0x7ff3, 0x7ff1, 0x7fee, 0x7fec, 0x7fe9, 0x7fe7, 0x7fe4, 0x7fe2, 
0x7fe1, 0x7fdf, 0x7fdf, 0x7fdf, 0x7fe0, 0x7fe2, 0x7fe6, 0x7fea, 
0x7ff0, 0x7ff8, 0x0002, 0x000d, 0x001a, 0x0029, 0x003b, 0x004e, 
0x0064, 0x007c, 0x0095, 0x00b2, 0x00d0, 0x00ef, 0x0111, 0x0134, 
0x0159, 0x017f, 0x01a6, 0x01cd, 0x01f5, 0x021d, 0x0245, 0x026c, 
0x0292, 0x02b7,  0x02db, 0x02fd, 0x031c, 0x0339, 0x0354, 0x036b, 
0x0380, 0x0390, 0x039e, 0x03a7, 0x03ad, 0x03af, 0x7feb, 0x7ff6},
{0x17, 0x0f, 0x13, 0x17, 0x1c, 0x20, 0x24, 0x27, 
0x29, 0x2b, 0x2a, 0x28, 0x23, 0x1d, 0x13, 0x08, 
0x7ff9, 0x7fe8, 0x7fd5, 0x7fc0, 0x7fa9, 0x7f92, 0x7f7a, 0x7f63,
0x7f4d, 0x7f3a, 0x7f2b, 0x7f20, 0x7f1b, 0x7f1c, 0x7f25, 0x7f37,
0x7f52, 0x7f77, 0x7fa7, 0x7fe1, 0x25, 0x73, 0xcb, 0x12b, 
0x193, 0x201, 0x273, 0x2e9, 0x35f, 0x3d4, 0x447, 0x4b4,
0x51a, 0x576, 0x5c8, 0x60e, 0x645, 0x66d, 0x686, 0x68e},
{0x7fed, 0x7ffc, 0x7ffc, 0x7ffe, 0x00, 0x04, 0x09, 0x0f, 
0x16, 0x1e, 0x27, 0x30, 0x38, 0x40, 0x46, 0x4a, 
0x4c, 0x4a, 0x43, 0x39, 0x2a, 0x15, 0x7ffc, 0x7fdf, 
0x7fbe, 0x7f9b, 0x7f75, 0x7f50, 0x7f2d, 0x7f0d, 0x7ef3, 0x7ee0, 
0x7ed8, 0x7edb, 0x7eec, 0x7f0c, 0x7f3c, 0x7f7d, 0x7fd0, 0x34, 
0xa8, 0x12a, 0x1ba, 0x253, 0x2f4, 0x399, 0x43e, 0x4e0, 
0x57a, 0x609, 0x688, 0x6f6, 0x74e, 0x78f, 0x7b7, 0x7c4}, 
{0x01, 0x7fe9, 0x7fed, 0x7fe8, 0x7fe2, 0x7fdc, 0x7fd7, 0x7fd4, 
0x7fd2, 0x7fd3, 0x7fd7, 0x7fdf, 0x7fea, 0x7ff9, 0x0b, 0x20, 
0x36, 0x4e, 0x64, 0x78, 0x88, 0x93, 0x96, 0x90, 
0x81, 0x67, 0x43, 0x15, 0x7fdf, 0x7fa2, 0x7f62, 0x7f21, 
0x7ee4, 0x7eae, 0x7e84, 0x7e6a, 0x7e65, 0x7e77, 0x7ea5, 0x7eef, 
0x7f57, 0x7fdc, 0x7e, 0x139, 0x208, 0x2e8, 0x3d2, 0x4bf, 
0x5a7, 0x684, 0x74d, 0x7fd, 0x88c, 0x8f7, 0x938, 0x94e}
};

const static uint32_t adec_ddfs_freq[13] = { 
0x15555, 
0x1a12f, 
0x1a12f, 
0x15555, 
0x15555, 
0x1ed09, 
0x1ed09, 
0x1ed09, 
0x1ed09, 
0x1a12f, 
0x1a12f, 
0x1c715, 
0x8000};

typedef struct {
	uint32_t width;
	uint32_t height;

	uint32_t hscale_p;										
	uint32_t hscale_r;
	uint32_t hscale_d_m1;
	uint32_t hscale_ores_m1;
	uint32_t hscale_frac_lut0;			
	uint32_t hscale_frac_lut1;			
	uint32_t h_trig_thrd;										 
	
	uint32_t vscale_p;						
	uint32_t vscale_r;
	uint32_t vscale_d_m1;
	uint32_t vscale_ores_m1;
	uint32_t vscale_frac_lut0;			
	uint32_t vscale_frac_lut1;			

	uint32_t vscale_p_ntsc;				
	uint32_t vscale_r_ntsc;
	uint32_t vscale_d_m1_ntsc;
	uint32_t vscale_ores_m1_ntsc;
	uint32_t vscale_frac_lut0_ntsc;	
	uint32_t vscale_frac_lut1_ntsc;

	uint32_t v_trig_thrd;					

	uint32_t fb_cfg;													
	
	uint32_t f0_last_line_6p75;		
	uint32_t f1_last_line_6p75;		
	uint32_t last_line_6p75;			
	uint32_t last_pixel_6p75;			

	uint32_t f0_last_line_13p5;
	uint32_t f1_last_line_13p5;
	uint32_t last_line_13p5;
	uint32_t last_pixel_13p5;
	
	uint32_t f0_last_line_27;
	uint32_t f1_last_line_27;
	uint32_t last_line_27;
	uint32_t last_pixel_27;
} tNmiScalePara;

static tNmiScalePara szpar[] = {
	{320, 240,
		10, 1, 1, 319, 0xb9865320, 0x65320ec, 319, 
		5, 1, 0, 239, 0x630c9630, 0xc9630c9,
		5, 1, 0, 239, 0x630c9630, 0xc9630c9, 
		239, 
		19199, 
		(3+239), (3+239), (3+239), (4+(320*2)),
		(3+239), (3+239), (3+239), (4+(320*2)),
		(3+239), (3+239), (3+239), (4+(320*2))},
	{80, 60,
		10, 1, 7, 79, 0xba865320, 0x000000ed, 79, 
		8, 1, 3, 63, 0xeca86420, 0x00000000,
		4, 1, 2, 63, 0x0000c840, 0x00000000, 
		63, 
		1279, 
		(3+63), (3+63), (3+63), (4+(80*2)),
		(3+63), (3+63), (3+63), (4+(80*2)),
		(3+63), (3+63), (3+63), (4+(80*2))},
	{176, 144,
		4, 0, 3, 175, 0, 0, 175,
		2, 0, 1, 143, 0, 0, 
		3, 2, 0, 143, 0xb50, 0, 
		143,
		6335,
		(3+143), (3+143), (3+143), (4+(176*2)),
		(3+143), (3+143), (3+143), (4+(176*2)),
		(3+143), (3+143), (3+143), (4+(176*2))},
	{220, 176,
		15, 1, 2, 219, 0x76543210, 0x0fedcba9, 219,
		11, 7, 0, 175, 0x0eb97520, 0x0, 
		11, 4, 0, 175, 0xa9764310, 0xfdc, 
		175,
		9679,
		(3+175), (3+175), (3+175), (4+(220*2)),
		(3+175), (3+175), (3+175), (4+(220*2)),
		(3+175), (3+175), (3+175), (4+(220*2))},
	{240, 180,
		15, 7, 1, 239, 0x76543210, 0x0fedcba9, 239,
		5, 3, 0, 179, 0x000da630, 0x0, 
		3, 1, 0, 179, 0x00000b50, 0x0, 
		179,
		10799,
		(3+179), (3+179), (3+179), (4+(240*2)),
		(3+179), (3+179), (3+179), (4+(240*2)),
		(3+179), (3+179), (3+179), (4+(240*2))},
	{426, 320,
		10, 1, 1, 319, 0xb9865320, 0x65320ec, 319, 
		5, 1, 0, 239, 0x630c9630, 0xc9630c9,
		5, 1, 0, 239, 0x630c9630, 0xc9630c9, 
		239, 
		19199, 
		(3+319), (3+319), (3+319), (4+(426*2)),
		(3+319), (3+319), (3+319), (4+(426*2)),
		(3+319), (3+319), (3+319), (4+(426*2))},
	{360, 240,
		10, 1, 1, 319, 0xb9865320, 0x65320ec, 319, 
		5, 1, 0, 239, 0x630c9630, 0xc9630c9,
		5, 1, 0, 239, 0x630c9630, 0xc9630c9, 
		239, 
		19199, 
		(3+239), (3+239), (3+239), (4+(360*2)),
		(3+239), (3+239), (3+239), (4+(360*2)),
		(3+239), (3+239), (3+239), (4+(360*2))},
	{480, 280,
		10, 1, 1, 319, 0xb9865320, 0x65320ec, 319, 
		5, 1, 0, 239, 0x630c9630, 0xc9630c9,
		5, 1, 0, 239, 0x630c9630, 0xc9630c9, 
		239, 
		19199, 
		(3+279), (3+279), (3+279), (4+(480*2)),
		(3+279), (3+279), (3+279), (4+(480*2)),
		(3+279), (3+279), (3+279), (4+(480*2))},
	{480, 360,
		10, 1, 1, 319, 0xb9865320, 0x65320ec, 319, 
		5, 1, 0, 239, 0x630c9630, 0xc9630c9,
		5, 1, 0, 239, 0x630c9630, 0xc9630c9, 
		239, 
		19199, 
		(3+359), (3+359), (3+359), (4+(480*2)),
		(3+359), (3+359), (3+359), (4+(480*2)),
		(3+359), (3+359), (3+359), (4+(480*2))},
}; 		


#define ISNM600(id) (((id & 0xfff00) == 0x60000) ? 1 : 0) 
#define ISNM600B0(id) (((id & 0xfffff) == 0x600b0) ? 1 : 0)
#define ISNM600C0(id) (((id & 0xfffff) == 0x600c0) ? 1 : 0)
#define ISNM601(id) (((id & 0xfff00) == 0x60100) ? 1 : 0) 
#define ISNM601A(id) (((id & 0xffff0) == 0x601a0) ? 1 : 0) 
#define ISNM601A0(id) (((id & 0xfffff) == 0x601a0) ? 1 : 0)
#define ISNM601A1(id) (((id & 0xfffff) == 0x601a1) ? 1 : 0)
#define ISNM601B(id) (((id & 0xffff0) == 0x601b0) ? 1 : 0)
#define ISNM601B0(id) (((id & 0xfffff) == 0x601b0) ? 1 : 0)
#define ISNM602B0(id) (((id & 0xfffff) == 0x602b0) ? 1 : 0)	


#define UGAIN 0x2f		
#define VGAIN 0x21		
#define USCALE 0x2f	
#define VSCALE 0x21	
#define UVSNRH 10.		
#define UVSNRL -5.

#define SECAM_UGAIN 0x35		
#define SECAM_VGAIN 0x3cd	
#define SECAM_USCALE 0x35	
#define SECAM_VSCALE -51

static void atvRfburstWrite(uint32_t sadr, const uint8_t *b, int cnt)
{
	uint8_t wc;
	int retry,i, ix;
	uint32_t val32;

    if(chip.buserror == 1) 
    {     
		nmi_debug(N_ERR, "atvRfburstWrite--error, need reset!!!");
		return;
    }	
	
	ix = 0;
	do {
		if (cnt >= 7)
			wc = 7;
		else
			wc = cnt;

		wReg32(0x20710, (sadr+ix));
		for(i = 0; i < (wc); i++) {
			wReg32(0x20710, b[i + ix]);
		}

		wReg32(0x20700, 0x2);

		retry = 100;
		do {
			val32 = rReg32(0x2072c);
			if ((val32 >> 9) & 0x1)
				break;
		} while (retry--);
		
		val32 = rReg32(0x20760);
		if (!(val32 & 0x1)) {
			nmi_debug(N_ERR, "RF Burst write: Fail, sadr (%02x), val (%08x)\n", sadr, val32);
			chip.buserror = 1;
		}

		cnt -= wc;
		ix += wc;
	} while (cnt > 0);


	return;
}

static uint8_t atvrfread(uint32_t adr)
{
	int retry;
	uint32_t val32;

    if(chip.buserror == 1) 
    {     
		nmi_debug(N_ERR, "atvrfread--error, need reset!!!");
		return;
    }	


	wReg32(0x20710, adr);
	wReg32(0x20700, 2);

	retry = 100;
	do {
		val32 = rReg32(0x2072c);
		if (val32 & 0x200)
			break;
	} while (retry--);

	if (retry <= 0) {
		nmi_debug(N_ERR, "RF Read reg: Fail, wait STOP DET intr, (%08x)\r\n", val32);
		chip.buserror = 1;
		return 0;
	}
	wReg32(0x20710, 0x100);
	wReg32(0x20700, 2);

	retry = 100;
	do {
		val32 = rReg32(0x2072c);		
		if (val32 & 0x200)
			break;
	} while (retry--);

	if (retry <= 0) {
		nmi_debug(N_ERR, "RF Read reg: Fail, wait STOP DET intr, (%08x)\r\n", val32);
		chip.buserror = 1;
		return 0;
	}

	rReg32(0x20760);
	val32 = rReg32(0x20710);		

	return (uint8_t)val32;
}

static void atvrfwrite(uint32_t adr, uint8_t val8)
{
	int retry;
	uint32_t val32;

    if(chip.buserror == 1) 
    {     
		nmi_debug(N_ERR, "atvrfwrite--error, need reset!!!");
		return;
    }	

	wReg32(0x20710, adr);
	wReg32(0x20710, val8);
	wReg32(0x20700, 2);

	retry = 100;
	do {
		val32 = rReg32(0x2072c);
		if (val32 & 0x200)
			break;
	} while (retry--);

	if (retry <= 0) {
		nmi_debug(N_ERR, "RF Write reg: Fail, wait STOP DET intr, (%08x)\r\n", val32);
		chip.buserror = 1;
		return;
	}

	rReg32(0x20760);	

	return;
}


static void atv_enable_notch(int index, int enable)
{
	uint32_t val32;
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;

	if (ISNM600(pp->chipid)) {
		val32 = rReg32(0x2ee08);
		if (enable)
			val32 &= ~(1 << 6);		
		else
			val32 |= (1 << 6);	
		wReg32(0x2ee08, val32);
	} else {
		val32 = rReg32(0x2ee04);
		if (enable)
			if (ISNM601B(pp->chipid))
				val32 &= ~(1 << 6);
			else
				val32 &= ~(1 << 16);
		else
			if (ISNM601B(pp->chipid))
				val32 |= (1 << 6);
			else
				val32 |= (1 << 16);
		wReg32(0x2ee04, val32);
	}
}

static void atv_notch_ctl(int en)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;

	if (en) {
		if (!pp->notchOn) {
			atv_enable_notch(1, 1);
			nmi_debug(N_INFO, "*** Notch Enable ***\n");
		}
		pp->notchOn = 1;
	} else {
		if (pp->notchOn) {
			atv_enable_notch(1, 0);
			nmi_debug(N_INFO, "*** Notch Disable ***\n");
		}
		pp->notchOn = 0;
	}

	return;
}

static void atv_set_notch_frequency(int index, double freq)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32, reg_i, reg_q;
	double foff, ppm, temp, fsamp;
	int temp2;

	foff = 0;
	if (pp->standard != nFM_RADIO) {
		val32 = rReg32(0x2fe10) & 0xffff;
		ppm =  (((1 << 16) * 0.5) / ((double)val32)) - 1;
	} else 
		ppm = pp->plloffset;
	
	if (ISNM601B(pp->chipid))
		fsamp = 13.5 * (1 + ppm);
	else
		fsamp = 27 * (1 + ppm);

	 /*lint -e628*/
	temp = cos(2 * 3.14156 * ((freq + foff) / fsamp)); /*lint -esym(628,cos)*/
	temp2 = (int) (temp * (1<<20));
	reg_i = (temp2 < 0) ? ( (1 << 22) + temp2) : temp2;

	temp = sin(2 * 3.14156 * ((freq + foff) / fsamp)); /*lint -esym(628,sin)*/
	temp2 = (int) (temp * (1<<20));
	reg_q = (temp2 < 0) ? ( (1 << 22) + temp2) : temp2;
    /*lint +e628*/
	wReg32(0x2ee40, reg_i);
	wReg32(0x2ee44, reg_q);
	nmi_debug(N_VERB, "Notch: i (%08x), q (%08x)\n", reg_i, reg_q);
}

static void atv_notch_check(void)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	int rssi;

	if (ISNM601B0(pp->chipid)) {
		if (pp->notch_apply) {
			rssi = atv_fe_get_rssi();
			if (rssi < pp->notch_rssi)
				atv_notch_ctl(1);
			else	
				atv_notch_ctl(0);
		}
	}


}

static void atv_notch_width(int width)
{
	uint32_t val32;
	
	val32 = rReg32(0x2ee3c);
	val32 &= ~(0x7 << 3);
	val32 |= ((width&0x7) << 3);
	wReg32(0x2ee3c, val32);
}

static void atv_notch_frequency(uint32_t spur, uint32_t lo)
{
	double frequency;

	frequency = ((double)spur - (double)lo)/1000000;
	atv_set_notch_frequency(1, frequency);
}

static void atv_notch_spur(tNotchPara *spur, int size)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	int i, notch = 0, wd = 0, local = 0;
	double luma, bh, bl, sf;

	luma = (double)pp->frequency/1000000;

	if ((pp->standard == nNTSC) || (pp->standard == nPAL_M) || (pp->standard == nPAL_N)) {
		bh = luma + 4.5 + 0.75;
	  	bl = luma - 1.25;
	} else if ((pp->standard == nPAL_B) || (pp->standard == nSECAM_B)) {
	   bh = luma + 5.5 + 0.75;
	  	bl = luma - 1.25;
	} else {
		if (pp->standard == nFM_RADIO) {
			bh = luma + 0.2;
			bl = luma - 0.2;
		} else {
		  	bh = luma + 6.5 + 0.75;
		 	bl = luma - 1.25;
		}
	}

	for (i = 0; i < size; i++) {
		sf = (double)spur[i].sfrequency/1000000; 
		if ((sf >= bl) && (sf <= bh)) {	
			notch = 1;
			wd = spur[i].width;
			local = spur[i].local;
			break;
		}
	}
	
	if (notch) {
		double offset = 0.;

		atv_notch_ctl(0);

		if (!local) {
			offset = atv_demod_freq_offset();
		}

		atv_notch_width(wd);					

		atv_notch_frequency((uint32_t)(sf*1000000), (uint32_t)(pp->lofrequency + offset));

		atv_notch_ctl(1);

		pp->notch_apply = 1;
		pp->notch_fq = (uint32_t)(sf * 1000000);

	} 
}

static void atv_notch_set(int en, uint32_t frequency, int width)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;

	if (en) {
		atv_notch_width(width);
		atv_notch_frequency(frequency, pp->lofrequency);
		atv_notch_ctl(1);
	} else {
		atv_notch_ctl(0);
	}
}

static double atv_get_notch_frequency(void)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	double fq;

	if (pp->notch_apply)
		fq = (double)pp->notch_fq/1000000;
	else
		fq = 0.;

	return fq;
}

static uint32_t atv_calc_pll(double offset)
{
	uint32_t clk = chip.inp.xo;
	double d0;
	uint32_t n0, n1, pll;
	
	d0 = (double)(216 * (1 + (offset * 0.01)))/(double)clk;
	n0 = (uint32_t)d0;
	n1 = (uint32_t)(((d0 - n0) * (1 << 20)) + 0.5);
	pll = (2 << 26) | ((n0 & 0xff) << 20) | ((n1 & 0xfffff));

	nmi_debug(N_VERB, "pll (%08x)\n", pll);
	return pll; 
}

static void atv_update_pll(void)
{
	uint32_t val1, val3;
	int retry;
	uint32_t val2;

	val3 = rReg32(0x20f54);
	val1 = val3 & ~((7 << 16) | (7 << 20));
	wReg32(0x20f54, val1);

	val1 = rReg32(0x20f60);
	val1 |= 0x1;
	wReg32(0x20f60, val1);

	retry = 1000;	
	do {
		val2 = rReg32(0x21906);
		if ((val2 & 0x3) == 3) {
			nmi_debug(N_VERB, "PLL Update: (%08x), retry (%d)\n", val2, retry);
			break;
		}
	} while (retry--);	

	if (retry <= 0) {
		nmi_debug(N_ERR, "Failed PLL update, (%08x)...\n", val2);
	}

	val1 &= ~0x1;
	wReg32(0x20f60, val1);

	wReg32(0x20f54, val3);
	
	return;
}


static void atv_apply_spur_offset(double off)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;

	pp->apply = 1;
	pp->applyoffset = off;
}

static double atv_get_spur_offset(void)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;
	return pp->plloffset;
}

static void pll_move_algo(int luma_freq_int, tNmiTvStandard std,int bandwidth, double prefered_spur_offset, double * pold_spur, double * pnew_spur, double *pshift_prefered_amount,int * pnotch_on,double *pshift_amount, int *pnotch_bw,int *puse_DAC)
{
    double ch_low,ch_high,ch_medium;
    double old_spur, new_spur, shift_amount;
    double prefered_spur;
    int notch_on;
    double spur0,spur1;
    int no_inband_spur=0;
    int move_in_band_ntsc=0;
    double d0,d1;
    double luma_freq;
    double non_move_margin=0;
    double mini_move=0.003;
    double prefered_sh;
    double spur_out;
    double alt_sh;
    int har_order;
    int alow_0_move=0;
    int notch_bw=0;
    int use_DAC = 0;
    luma_freq=luma_freq_int/1.0e6;
    
    notch_on=0;
    
    if (bandwidth==6)
    {
        ch_high=luma_freq+4.5+0.75;
        ch_low=luma_freq-1.25;
    }
    else if (bandwidth==7)
    {
        ch_high=luma_freq+5.5+0.75;
        ch_low=luma_freq-1.25;
    }
    else
    {
        ch_high=luma_freq+6.5+0.75;
        ch_low=luma_freq-1.25;
    }
    
    ch_medium=luma_freq+prefered_spur_offset;
  /*lint -e628*/
    spur0=floor(ch_medium/6.75)*6.75;/*lint -esym(628,floor)*/
    spur1=floor(ch_high/6.75)*6.75;/*lint -esym(628,floor)*/
    /*lint +e628*/
    if ((spur0<=ch_low) && (spur1<=ch_low))
        no_inband_spur=1;
    if ((spur0<=ch_low) && (spur1>=ch_high))
        no_inband_spur=2;
    
    if ((no_inband_spur!=0) && (alow_0_move==1))
    {
         shift_amount=0;
         if (no_inband_spur==1) {
             /*lint -e628*/
             d0=fabs(ch_low-spur1);/*lint -esym(628,fabs)*/
             /*lint +e628*/
             d1=fabs(ch_high-spur1-6.75);
             new_spur=(d0>=d1)?spur1-6.75:spur1;
         } else {
             d0=fabs(ch_low-spur0);
             d1=fabs(ch_high-spur1);
             new_spur=(d0>=d1)?spur0:spur1;
         }
        old_spur=new_spur;
        notch_on=1;
        prefered_sh=0;
    }
    else
    {
        if ((bandwidth==6) && (move_in_band_ntsc==0))
        {            
            spur1=floor((luma_freq+4.5+0.9)/6.75)*6.75;
            if (spur1==luma_freq+4.5+0.9)
            {
                spur0=spur1-6.75;
                new_spur=spur1;
                old_spur= spur0;
                notch_on=0;                
                shift_amount=0.0;
                prefered_sh=shift_amount;
           
            }
            else
            {
                shift_amount=(luma_freq+4.5+0.9-spur1)/spur1;
                old_spur=spur1;  
                new_spur=spur1*(1+shift_amount);
                notch_on=0;
                prefered_sh=  shift_amount;
              
            }
            if ((shift_amount<mini_move))                
            {                
             
                alt_sh=(luma_freq+4.5+0.9-(spur1-6.75))/(spur1-6.75);
                if (alt_sh<=0.05)
                {
                    prefered_sh=alt_sh;
                    new_spur=(spur1-6.75)*(1+ prefered_sh);
                }
            }
        }
        else
        {
            notch_on=1;
            prefered_spur=luma_freq+prefered_spur_offset;
            
            if (spur1>(prefered_spur+non_move_margin))
            {
                d0=spur0-ch_low;
                d1=ch_high-spur1;
                
                if (d0>d1)
                    old_spur=spur0;
                else
                    old_spur=spur1;
                
                spur_out=spur0;
            }
            else 
            {
                spur_out=spur1;
                old_spur=spur1;
            }
                 
             new_spur=prefered_spur;
             shift_amount=(new_spur-spur_out)/spur_out;
             
             prefered_sh=  shift_amount;
             
            
             
             
             har_order=(int)floor(spur_out/6.75+0.1);
             
             if ((har_order%2)==0)
             {
               alt_sh=(new_spur-(spur_out-6.75))/(spur_out-6.75);
               if (alt_sh<=0.050)
               {
                   prefered_sh=alt_sh;
               }                                 
             }
             else
             {
                 if  (shift_amount<mini_move)
                 {
                      alt_sh=(new_spur-(spur_out-6.75*2))/(spur_out-6.75*2);
                      if (alt_sh<=0.050)
                      {                      
                           prefered_sh=alt_sh;
                      }                      
                 }
                 
             }
                  
             

        }
       
    }
    
    
    if   ((luma_freq_int==48250000)  && (bandwidth==7)   )
    {
         shift_amount=0.05;
        new_spur= 52;
        old_spur=54;       
           prefered_sh=shift_amount;
    }
    else if ((luma_freq_int==55250000)  && (bandwidth==7)   )
    {
         shift_amount=0.0;
        new_spur= 54;
        old_spur=54;       
           prefered_sh=shift_amount;
           
    }
    else if ((luma_freq_int==62250000)  && (bandwidth==7)   )
    {
         shift_amount=0.05;
        new_spur= 60.75*(1+shift_amount);
        old_spur=57.5;       
           prefered_sh=shift_amount;
    }
    
    if (std!=nPAL_I)
    {
        if ((luma_freq_int==49750000) && (bandwidth==8)   )
        {
            shift_amount=0.05;
            new_spur=52;
            old_spur=54;
            prefered_sh=shift_amount;
        }
        else if  ((luma_freq_int==57750000)  && (bandwidth==8)   )
        {
            
            shift_amount=(57.75+5.5- 60.75)/60.75;
            new_spur= 9*6.75*(1+shift_amount);
            old_spur= 9*6.75*1.0;
            prefered_sh=shift_amount;
            notch_on=1;
        }
        else if   ((luma_freq_int==77250000)  && (bandwidth==8)   )
        {
            shift_amount=(77.25+5.5- 81)/81;
            new_spur= 12*6.75*(1+shift_amount);
            old_spur=12*6.75*1.0;
            prefered_sh=shift_amount;
        }
    }
    else
    {
         if ((luma_freq_int==49750000) && (bandwidth==8)   )
        {
            shift_amount=0.05;
            new_spur=52;
            old_spur=54;
            prefered_sh=shift_amount;
        }        
        else if ((luma_freq_int== 61750000) && ((std==nPAL_I)  || (std==nPAL_I)) )
        {
            shift_amount=0.05;
            new_spur=9*6.75*1.05;
            old_spur=67.5;
            prefered_sh=shift_amount;
        }
          else if  ((luma_freq_int==57750000)  && (bandwidth==8)   )
        {
            
            shift_amount=(57.75+5.5- 60.75)/60.75;
            new_spur= 9*6.75*(1+shift_amount);
            old_spur= 9*6.75*1.0;
            prefered_sh=shift_amount;
            notch_on=1;

        }
          else if   ((luma_freq_int==77250000)  && (bandwidth==8)   )
        {
            shift_amount=(77.25+5.5- 81)/81;
            new_spur= 12*6.75*(1+shift_amount);
            old_spur=12*6.75*1.0;
            prefered_sh=shift_amount;
        }
    }

   
         
    if ((luma_freq_int== 49250000 ) && (bandwidth==6) )
    {
        shift_amount=.013;
        new_spur=52;
        old_spur=54;   
           prefered_sh=shift_amount;
           notch_on=1;
    }
    else if ((luma_freq_int== 51250000 )&& (bandwidth==6) )
     {
        shift_amount=.05;
        new_spur=52;
        old_spur=54;   
        notch_on=0; 
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 53250000) && (bandwidth==6) )
     {
        shift_amount=(53.25+prefered_spur_offset-54)/54;
        new_spur=54*(1+shift_amount);
        old_spur=54;   
        prefered_sh=shift_amount;
        notch_on=1;
     }
     else if ((luma_freq_int== 55250000) && (bandwidth==6) )
     {
        shift_amount=0.0;
        new_spur=54;
        old_spur=54;   
        prefered_sh=shift_amount;
        notch_on=1;
     }
      else if ((luma_freq_int== 57250000 )&& (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=.032;
        new_spur=9*6.75*(1+shift_amount);
        old_spur=9*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 59250000 )&& (bandwidth==6) )
     {
        shift_amount=(59.25+prefered_spur_offset-60.75)/60.75;
        new_spur=9*6.75*(1+shift_amount);
        old_spur=9*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 61250000 ) && (bandwidth==6) )
     {
         
        shift_amount=(61.25+prefered_spur_offset-60.75)/60.75;
        new_spur=9*6.75*(1+shift_amount);
        old_spur=9*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
           
         
     }
     else if ((luma_freq_int== 63250000 ) && (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=.017;
        new_spur=10*6.75*(1+shift_amount);
        old_spur=10*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
      else if ((luma_freq_int== 65250000 ) && (bandwidth==6)&& (move_in_band_ntsc==1))
     {
        shift_amount=.048;
        new_spur=10*6.75*(1+shift_amount);
        old_spur=10*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 67250000 ) && (bandwidth==6) )
     {
        shift_amount=(67.25+prefered_spur_offset-67.5)/67.5;
        new_spur=10*6.75*(1+shift_amount);
        old_spur=10*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
       else if ((luma_freq_int== 69250000 ) && (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=0.006;
        new_spur=11*6.75*(1+shift_amount);
        old_spur=11*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
       else if ((luma_freq_int== 71250000 ) && (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=0.034;
        new_spur=11*6.75*(1+shift_amount);
        old_spur=11*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
    else if ((luma_freq_int== 73250000 ) && (bandwidth==6) )
     {
        
         shift_amount=(73.25+prefered_spur_offset-74.25)/74.25;
        new_spur=11*6.75*(1+shift_amount);
        old_spur=11*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 75250000 ) && (bandwidth==6) )
     {
        
         shift_amount=(75.25+prefered_spur_offset-74.25)/74.25;
        new_spur=11*6.75*(1+shift_amount);
        old_spur=11*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
       else if ((luma_freq_int== 77250000 ) && (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=0.021;
        new_spur=12*6.75*(1+shift_amount);
        old_spur=12*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 79250000 ) && (bandwidth==6) )
     {
        shift_amount=(79.25+5.4)/81-1;
        new_spur=78;
        old_spur=81;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
      else if ((luma_freq_int== 81250000 )&& (bandwidth==6) )
     {
        shift_amount=(81.25+prefered_spur_offset-81)/81;
        new_spur=12*6.75*(1+shift_amount);
        old_spur=12*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
      else if ((luma_freq_int== 83250000 )&& (bandwidth==6) && (move_in_band_ntsc==1) )
     {
        shift_amount=0.011;
        new_spur=13*6.75*(1+shift_amount);
        old_spur=13*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
      else if ((luma_freq_int== 85250000 ) && (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=0.034;
        new_spur=13*6.75*(1+shift_amount);
        old_spur=13*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 87250000 )&& (bandwidth==6) )
     {
        shift_amount=(87.25+prefered_spur_offset-87.75)/87.75;
        new_spur=13*6.75*(1+shift_amount);
        old_spur=13*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 89250000 )&& (bandwidth==6) )
     {
        shift_amount=(89.25+prefered_spur_offset-87.75)/87.75;
        new_spur=13*6.75*(1+shift_amount);
        old_spur=13*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 91250000 )&& (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=0.023;
        new_spur=14*6.75*(1+shift_amount);
        old_spur=14*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
         else if ((luma_freq_int== 95250000 )&& (bandwidth==6) )
     {
          shift_amount=(95.25+prefered_spur_offset-94.5)/94.5;
        new_spur=14*6.75*(1+shift_amount);
        old_spur=14*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 97250000 ) && (bandwidth==6)&& (move_in_band_ntsc==1) )
     {
        shift_amount=0.014;
        new_spur=15*6.75*(1+shift_amount);
        old_spur=15*6.75;   
        notch_on=0;
           prefered_sh=shift_amount;
     }
     else if ((luma_freq_int== 101250000 ) && (bandwidth==6) )
     {
         shift_amount=(101.25+prefered_spur_offset-101.25)/101.25;
        new_spur=15*6.75*(1+shift_amount);
        old_spur=15*6.75;   
        notch_on=1;
           prefered_sh=shift_amount;
     }
      else if ((luma_freq_int== 109250000 ) && (bandwidth==6) )
     {
         shift_amount=0;
        new_spur=108*(1+shift_amount);
        old_spur=108;   
        notch_on=1;
        prefered_sh=shift_amount;
        
        
     }

    if ((luma_freq_int== 61250000 ) && (bandwidth==6) )
	{
		shift_amount=0;
        new_spur=60.75;
        old_spur=60.75;   
        notch_on=0;
        prefered_sh=shift_amount;
	} 
    
    if ((luma_freq_int== 45750000 ) && (bandwidth==8) )
	{
		shift_amount=0;
        new_spur=47.25;
        old_spur=47.25;   
        notch_on=0;
        prefered_sh=shift_amount;
	}
	
    if ((luma_freq_int== 59750000 ) && (bandwidth==8) )
	{
		shift_amount=0;
        new_spur=60.75;
        old_spur=60.75;   
        notch_on=0;
        prefered_sh=shift_amount;
	}
    
    if ((luma_freq_int== 55250000 ) && (bandwidth==7) )
	{
		shift_amount=0.01;
        new_spur=60.75*(1+shift_amount);
        old_spur=60.75;   
        notch_on=0;
        prefered_sh=shift_amount;
	} 
    if ((luma_freq_int== 62250000 ) && (bandwidth==7) )
	{
		shift_amount=0.009;
        new_spur=67.5*(1+shift_amount);
        old_spur=67.5;   
        notch_on=0;
        prefered_sh=shift_amount;
	} 
	

    if ((luma_freq_int== 182250000 ) && (bandwidth==7) )
	{
		shift_amount=0.00;
        new_spur=182.25;
        old_spur=182.25;   
        notch_on=0;
        prefered_sh=shift_amount;
	} 
     if ((luma_freq_int== 210250000 ) && (bandwidth==7) )
	{
		shift_amount=0.00;
        new_spur=209.25;
        old_spur=209.25;   
        notch_on=0;
        prefered_sh=shift_amount;
	} 
    if ((luma_freq_int== 224250000 ) && (bandwidth==7) )
	{
		shift_amount=0.00;
        new_spur=229.5;
        old_spur=229.5;   
        notch_on=0;
        prefered_sh=shift_amount;
	} 
    
    if ((luma_freq_int== 77250000 ))
    {
       new_spur=78;
       notch_bw=7;
	   notch_on=1;
    }
    if ((luma_freq_int== 181250000 ))
    {
       new_spur=182;
       notch_bw=7;
	   notch_on=1;
    }
    if ((luma_freq_int== 753250000 ))
    {
       new_spur=754;
       notch_bw=7;
	   notch_on=1;
    }
    
    if ((luma_freq_int== 55250000 ) |(luma_freq_int== 65750000 )|(luma_freq_int== 53750000 )|(luma_freq_int== 48250000 )|(luma_freq_int== 62250000 )|(luma_freq_int== 57750000 ))
    {
        use_DAC=1;
    }
    if ((luma_freq_int== 49750000 ) |(luma_freq_int== 59250000 ))
    {
        use_DAC=1;
    }
    
    if (prefered_sh>0.051)
    {
        old_spur=0;
        new_spur=0;
        shift_amount=0;
        notch_on=0;
        prefered_sh=0;
    }
    
    *(pold_spur)=old_spur;
    *(pnew_spur)=new_spur;
    *(pshift_amount)=shift_amount;
    *(pnotch_on)=notch_on;
    *(pshift_prefered_amount)=prefered_sh;
    *(pnotch_bw)=notch_bw;
    *(puse_DAC)=use_DAC;
}

static double atv_calc_pll_off(tNmiTvStandard std)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	double doff, prefer_offset;
	int bandwidth, notch = 0, notchbw, usedac = 0;
	double o_spur, n_spur, sft_prefer, sft;

	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		if ((std == nNTSC) || (std == nPAL_M) || (std == nPAL_N)) {
			bandwidth = 6;
			prefer_offset = 2.0*0+1.9875;	
		} else if ((std == nPAL_B) || (std == nSECAM_B)) {
			bandwidth = 7;
			prefer_offset = 2.75*0+2.4875;
		} else {
			bandwidth = 8;
			prefer_offset = 2.75*0+2.9875;
		}

		pll_move_algo(pp->frequency, std, bandwidth, prefer_offset, &o_spur, &n_spur, &sft_prefer, &notch, &sft, &notchbw, &usedac);

		doff = (sft_prefer * 100);
		pp->notch_apply = notch;
		pp->notch_fq = (uint32_t)(n_spur * 1000000);
		pp->notch_bw = notchbw;
		pp->notch_dac = usedac;
	} else {
		doff = ((pp->digiclk/216000000) -1) * 100;

		pp->notch_apply = 0;

		if (std == nNTSC) {
			if (pp->frequency == 55250000) {
				pp->notch_apply = 1;
				pp->notch_fq = 55058000;
				pp->notch_bw = 2;
				pp->notch_rssi = -70;
			}
			if (pp->frequency == 77250000) {
				pp->notch_apply = 1;
				pp->notch_fq = 78000000;
				pp->notch_bw = 7;
				pp->notch_rssi = -70;
			}
			if (pp->frequency == 181250000) {
				pp->notch_apply = 1;
				pp->notch_fq = 182000000;
				pp->notch_bw = 7;
				pp->notch_rssi = -70;
			}
		} else if (std == nPAL_I) {
			if (pp->frequency == 53750000) {
				pp->notch_apply = 1;
				pp->notch_fq = 54580000;
				pp->notch_bw = 1;
				pp->notch_rssi = -63;
			}
			if (pp->frequency == 77250000) {
				pp->notch_apply = 1;
				pp->notch_fq = 78000000;
				pp->notch_bw = 7;
				pp->notch_rssi = -70;
			}
		} else if ((std == nPAL_B) || (std == nPAL_D)) {
			if ((pp->frequency == 48250000) || (pp->frequency == 49750000)) {
				pp->notch_apply = 1;
				pp->notch_fq = 53427000;
				pp->notch_bw = 1;
				pp->notch_rssi = -75;
			}
			if (pp->frequency == 55250000) {
				pp->notch_apply = 1;
				pp->notch_fq = 55530000;
				pp->notch_bw = 2;
				pp->notch_rssi = -70;
			}
			if (pp->frequency == 77250000) {
				pp->notch_apply = 1;
				pp->notch_fq = 77991000;
				pp->notch_bw = 0;
				pp->notch_rssi = -70;
			}
#if 1
			if (pp->frequency == 65750000) {
				pp->notch_apply = 1;
				pp->notch_fq = 68850000;
				pp->notch_bw = 2;
				pp->notch_rssi = -70;
			}
#endif

		} 

	}

	return doff;
}

static void atv_adj_pll(uint32_t frequency, tNmiTvStandard std)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t pll, n1, n2, val32;
	double off, dn1, dn2;

	if (!pp->apply) {
		if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
			if (std != nFM_RADIO) 
				off = atv_calc_pll_off(std);
			else 
				if (pp->frequency >= 107500000)
					off = 1;
				else
					off = 0;
		} else {
			off = atv_calc_pll_off(std);
		}
	} else {
		off = pp->applyoffset;
		pp->apply = 0; 
	}
	nmi_debug(N_VERB, "PLL Offset (%f)\n", off);

	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		if (pp->plloffset != off) {
			pll = atv_calc_pll(off);
			wReg32(0x20f14, pll);
			atv_update_pll();

			dn1 = 0.5 / ( 1 + (off * 0.01));
			dn2 = 1 / dn1;
			n1 =  (uint32_t)(dn1 * (1 << 16));
			n2 = (uint32_t)(dn2 * (1 << 6));
			val32 = (n1 & 0xffff) | ((n2 &  0xff) << 16);
			wReg32(0x2fe10, val32);
		}
	} else {
		atv_update_pll();

		dn1 = 0.5 / (1 + (off * 0.01));
		dn2 = 1 / dn1;
		n1 =  (uint32_t)(dn1 * (1 << 16) + 0.5);
		n2 = (uint32_t)(dn2 * (1 << 6) + 0.5);
		val32 = (n1 & 0xffff) | ((n2 &  0xff) << 16);
		wReg32(0x2fe10, val32);
	}

	if (pp->standard != std) {
		atv_fe_config(std, off);
		if (std != nFM_RADIO) {
			atv_vdec_config(std);
			atv_be_config(std);
		}
		atv_adec_config(std, off);
		pp->standard = std;
		pp->plloffset = off;
	} else {
		if (pp->plloffset != off) {
			atv_fe_config(std, off);
			atv_adec_config(std, off);
			pp->plloffset = off;
		}
	}  

	return;
}

static void atv_tuner_config(tAtvTune *tune)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;
	uint32_t		clk = chip.inp.xo * 1000000;
	uint32_t		dfrequency;
   	double 		target, vco;
	uint32_t		n0, alpha, r1, r2;
	uint8_t			val[4];

	if (pp->notchOn)
		atv_notch_ctl(0);

	atv_demod_reset_start(tune->standard);

	pp->frequency = dfrequency = tune->frequency;

	if (tune->standard == nPAL_G)
		tune->standard = nPAL_B;
	if (tune->standard == nSECAM_G)
		tune->standard = nSECAM_B;

	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		if (tune->standard == nFM_RADIO )
			dfrequency -= 1687500;
		else if ((tune->standard == nNTSC ) || (tune->standard == nPAL_M) || (tune->standard == nPAL_N)) 
			dfrequency += 1987500;
		else if ((tune->standard == nPAL_B) || (tune->standard == nSECAM_B)) 
			dfrequency += 2487500;
		else
			dfrequency += 2987500;
	} else {
		if (tune->standard == nFM_RADIO )
			dfrequency -= (1687500/4);
		else if ((tune->standard == nNTSC ) || (tune->standard == nPAL_M) || (tune->standard == nPAL_N)) 
			dfrequency += 2100000;
		else if ((tune->standard == nPAL_B) || (tune->standard == nSECAM_B)) 
			dfrequency += 2600000;
		else
			dfrequency += 3100000;
	}
	pp->lofrequency = dfrequency;

	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		if (dfrequency <= 75000000) {
			target = ((double)dfrequency/clk) * 48;
			r1 = 6;
			r2 = 48;
		} else if (dfrequency <= 150000000) {
			target = ((double)dfrequency/clk) * 24;
			r1 = 5;
			r2 = 24;
		} else if (dfrequency <= 300000000) {
			target = ((double)dfrequency/clk) * 12;
			r1 = 4;
			r2 = 12;
		} else if (dfrequency <= 450000000) {
			target = ((double)dfrequency/clk) * 8;
			r1 = 2;
			r2 = 8;
		} else {
			target = ((double)dfrequency/clk) * 4;
			r1 = 0;
			r2 = 4;
		}

		n0 = (uint32_t)(target);
		alpha = (uint32_t)((target - n0)*(0x1000000));
		alpha >>= 4;

		val[0] = (uint8_t)n0;
		val[1] = (uint8_t)(alpha & 0xff);
		val[2] = (uint8_t)((alpha >> 8) & 0xff);
		val[3] = (uint8_t)((alpha >> 16) & 0xff);
		atvRfburstWrite(0x1, val, 4);
		atvrfwrite(0x05, (uint8_t)(0xc0 | (r1 & 0x7)));

		vco = ((double)dfrequency/1000000) * r2;
		if (vco < 2200)
			atvrfwrite(0x20, 0xfc);
		else if (vco < 2600)
			atvrfwrite(0x20, 0xfa);
		else if (vco < 3000)
			atvrfwrite(0x20, 0xf8);
		else
			atvrfwrite(0x20, 0xf6);	
		
		if (dfrequency <= 150000000) {
			atvrfwrite(0x00, 0xdf);
		} else if (dfrequency < 450000000) {
			atvrfwrite(0x00, 0x9f);
		} else {
			atvrfwrite(0x00, 0x5f);
		}
	} else {
		uint32_t w, pll0, pll1;

    	if (dfrequency <= 68850000) {
			target = ((double)dfrequency/clk) * 96;
			r1 = 7;
			r2 = 96;
    	} else if (dfrequency <= 79000000) {
			target = ((double)dfrequency/clk) * 72;
			r1 = 6;
			r2 = 72;
    	} else if (dfrequency <= 118000000) {
			target = ((double)dfrequency/clk) * 48;
			r1 = 5;
			r2 = 48;
    	} else if (dfrequency <= 214000000) {
			target = ((double)dfrequency/clk) * 32;
			r1 = 4;
			r2 = 32;
    	} else if (dfrequency <= 237000000) {
			target = ((double)dfrequency/clk) * 24;
			r1 = 3;
			r2 = 24;
    	} else if (dfrequency <= 430000000) {
			target = ((double)dfrequency/clk) * 16;
			r1 = 2;
			r2 = 16;
    	} else {
			target = ((double)dfrequency/clk) * 8;
			r1 = 1;
			r2 = 8;
    	}
		n0 = (uint32_t)(target);
		alpha = (uint32_t)((target - n0)*(1 << 19));
		vco = ((double)n0 + ((double)alpha/(1 << 19))) * clk;
		w = (uint32_t)((vco/216000000) + 0.0001);
		if (w > 31)
			w = 31;
		else if (w < 16)
			w = 16;
		pll0 = ((w - 16) << 28) | (n0 << 19) | alpha;
		wReg32(0x20f14, pll0);
		pll1 = rReg32(0x20f18);
		pll1 &= ~(7<<13);
		pll1 |= (r1 << 13);
		wReg32(0x20f18, pll1);
		pp->digiclk = vco/w;

		if (dfrequency < 70000000) {
			atvrfwrite(0x24, 0x85);
			atvrfwrite(0x25, 0x38);
		} else if ((dfrequency >= 70000000) && (dfrequency < 110000000)) {
			atvrfwrite(0x24, 0x43);
			atvrfwrite(0x25, 0x34);
		} else if ((dfrequency >= 110000000) && (dfrequency < 230000000)) {
			atvrfwrite(0x24, 0x00);
			atvrfwrite(0x25, 0x30);
		} else {
			atvrfwrite(0x24, 0x0);
			atvrfwrite(0x25, 0x0);
		}

		val[0] = atvrfread(0x08);
		if (dfrequency < 80000000) {
			val[0] |= (1 << 6);
		} else {
			val[0] &= ~(1 << 6);
		}
		val[0] &= ~(1 << 7);
		atvrfwrite(0x08, val[0]);
		val[0] |= (1 << 7);
		atvrfwrite(0x08, val[0]);
	}
	atv_adj_pll(dfrequency, tune->standard);

	if (ISNM601(pp->chipid))
		atv_fe_use_local_agc(tune->standard);
	
	atv_demod_reset();

#ifdef PC_ENV
	atv_toggle_gpio(16);
#endif

	if (pp->notch_apply) {
		atv_notch_width(pp->notch_bw);
		atv_notch_frequency(pp->notch_fq, pp->lofrequency);
		if (ISNM601B0(pp->chipid)) {
			int rssi;
			rssi = atv_fe_get_rssi();
			if (pp->notch_rssi < rssi)
				atv_notch_ctl(1);
		} else 
			atv_notch_ctl(1);
	}

}

static void atv_rf_agc_gain(tAtvRFGain *pg)
{
	uint32_t gain, val1, val2;

	val1 = atvrfread(0x47);
	gain = (val1 & 0x1c) >> 2;
	nmi_debug(N_VERB, "RF: 0x47 (%02x)\n", val1);
	
	switch (gain) {
	case 0:
		pg->lnadb = 0.0;
		pg->lna = 0xee;
		break;
	case 1:
		pg->lnadb = 5.0;
		pg->lna = 0xef;
		break;
	case 3:
		pg->lnadb = 10.0;
		pg->lna = 0xf0;
		break;
	case 7:
		pg->lnadb = 17.0;
		pg->lna = 0xf1;
		break;
	default:
		pg->lnadb = 99.0;
		pg->lna = 0x00;
		break;
	}
	
	val2 = atvrfread(0x46);
	gain = (val2 << 3) + ((val1 & 0xe0) >> 5) ;
	nmi_debug(N_VERB, "RF: 0x46 (%02x)\n", val2);

	switch (gain) {
	case 0x7ff:
		pg->bblidb = 34.8;
		pg->bbli	= 0xf1;
		break;
	case 0x3ff:
		pg->bblidb = 28.6;
		pg->bbli 	= 0xf0;
		break;
	case 0x1ff:
		pg->bblidb = 22.8;
		pg->bbli 	= 0xef;
		break;
	case 0xff:
		pg->bblidb = 16.7;
		pg->bbli 	= 0xee;
		break;
	case 0x7f:
		pg->bblidb = 13.7;
		pg->bbli 	= 0xed;
		break;
	case 0x5f:
		pg->bblidb = 12.0;
		pg->bbli = 0xec;
		break;
	case 0x3f:
		pg->bblidb = 9.5;
		pg->bbli 	= 0xeb;
		break;
	case 0x2f:
		pg->bblidb = 7.5;
		pg->bbli 	= 0xea;
		break;
	case 0x1f:
		pg->bblidb = 4.5;
		pg->bbli	= 0xe9;
		break;
	case 0x17:
		pg->bblidb = 2.4;
		pg->bbli	= 0xe8;
		break;
	case 0xf:
		pg->bblidb = -0.9;
		pg->bbli = 0xe7;
		break;
	case 0xb:
		pg->bblidb = -3.5;
		pg->bbli	= 0xe6;
		break;	
	case 0x7:
		pg->bblidb = -7.0;
		pg->bbli	= 0xe5;
		break;
	case 0x5:
		pg->bblidb = -9.0;
		pg->bbli	= 0xe4;
		break;
	case 0x3:
		pg->bblidb = -12.3;
		pg->bbli	= 0xe3;
		break;
	case 0x2:
		pg->bblidb = -15.0;
		pg->bbli	= 0xe2;
		break;
	case 0x1:
		pg->bblidb = -18.0;
		pg->bbli	= 0xe1;
		break;
	case 0x0:
		pg->bblidb = -20.0;
		pg->bbli	= 0xe0;
		break;
	default:
		pg->bblidb = 99.0;
		pg->bbli	= 0x00;
		break;
	}

	return;	
}

static uint32_t atv_rf_sig_del_chk(int reset)
{
	uint8_t ind, val8;
	ind = atvrfread(0x43);
	if (((ind & 0x3) != 0) && reset) {
		val8 = atvrfread(0x1b);
		val8 |= 0x20; 
		atvrfwrite(0x1b, val8);
		val8 &= 0xdf; 
		atvrfwrite(0x1b, val8);
	}

	return ind;

}


static void atv_fe_init(void)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;

	if (!ISNM601B(pp->chipid))
		wReg32(0x2ef18, 0x2d8c4251);
	else
		wReg32(0x2ef18, 0xd8c4251);

	wReg32(0x2ee68, 0x3ff894d2);

	wReg32(0x2ee3c, 0x4);

	wReg32(0x2ec08, 0x2fff269);

	wReg32(0x2ef48, 0x40018);		
}

static void atv_fe_aci_ctl(int en)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	if (ISNM601A(pp->chipid)) {
		val32 = rReg32(0x2ee04);

		if (en) {
			val32 &= ~(1 << 17); 
		} else {
			val32 |= (1 << 17);
		}

		wReg32(0x2ee04, val32);
	}
}

static void atv_fe_config(tNmiTvStandard std, double pllshift)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	int pllix;
	uint32_t val32, upllshift;
	uint32_t aci_mode=0;
	uint32_t arj_mode=0;

	if (std == nSECAM_B) {
		std = nPAL_B;
	} else if (std == nSECAM_D) {
		std = nPAL_D;
	} else if (std == nSECAM_G) {
		std = nPAL_G;
	} else if (std == nSECAM_H) {
		std = nPAL_H;
	} else if (std == nSECAM_K) {
		std = nPAL_K;
	} else if (std == nSECAM_K1) {
		std = nPAL_K1;
	} 

	if (pllshift < 1.7)
		pllix = 0;
	else if (pllshift < 3.4)
		pllix = 1;
	else
		pllix = 2;

	switch (std) {
	case nNTSC:
	case nPAL_M:
	case nPAL_N:
	case nFM_RADIO:
		if (!ISNM601(pp->chipid))
			wRegBurst(0x2f008, (uint8_t *)&atv_av_sep_coefficents_vbw_4o2mhz_m_n_ntsc_fm[pllix][0], (48*4));
		break;
	case nPAL_I:
		if (!ISNM601(pp->chipid))
			wRegBurst(0x2f008, (uint8_t *)&atv_av_sep_coefficents_vbw_5o5mhz_i[pllix][0], (48*4));
		break;
	default:
		if (!ISNM601(pp->chipid))
			wRegBurst(0x2f008, (uint8_t *)&atv_av_sep_coefficents_vbw_5o0mhz_b_b1_g_h_d_d1_k_k1_l[pllix][0], (48*4));
		break;
	}

	if (ISNM601B(pp->chipid)) {
		switch (std) {  
		case nNTSC:
		case nPAL_M:
		case nPAL_N:
		case nFM_RADIO:
			if (pllshift<2.5)
				aci_mode=0;
			else
				aci_mode=1;
			if (pllshift<1.6667)
				arj_mode=0;
			else if (pllshift<3.33334)
				arj_mode=1;
			else
				arj_mode=2;
			break;
		case nPAL_B:
			if (pllshift<1.6667) {
				aci_mode=2;
				arj_mode=3;
			} else if (pllshift<3.33334) {
				aci_mode=3;
				arj_mode=4;
			} else {
				aci_mode=4;
				arj_mode=5;
			}
			break;
		default:
			if (pllshift<1.6667) {
				aci_mode=5;
				arj_mode=3;
			} else if (pllshift<3.33334) {
				aci_mode=6;
				arj_mode =4;
			} else {			
				aci_mode=7;
				arj_mode=5;
			}
			break;
		}
	}

	val32 = rReg32(0x2ee04);
	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		val32 &= ~((1 << 4) | (3 << 1));
		if (std != nFM_RADIO) {
			val32 |=	(atv_standard_params[std].frontend_standard & 0x1) << 4;
			val32 |=	(atv_standard_params[std].aci_mode & 0x3) << 1;
		}
	} else {
		val32 &= ~(7<<9);
		val32 &= ~(7<<12);
		val32 &= ~(1<<22);
	}
	if (ISNM600(pp->chipid)) {
		if (std == nFM_RADIO) {
		  	val32 |= (1 << 3);
		} else {
			val32 &= ~(1 << 3);
		}
	} else if (ISNM601A(pp->chipid)) {
		if (std == nFM_RADIO) {
		  	val32 |= (1 << 3);
			val32 |= (1 << 17);
		} else {
			val32 &= ~(1 << 3);
			val32 &= ~(1 << 17);
		}
	} else {
		val32 |= (aci_mode<<9);
		val32 |= (arj_mode<<12);
		if (std == nFM_RADIO) { 		
			val32 |= (1 << 22);	
			val32 |= (1 << 23);
			val32 |= (1 << 7);	
		} else {
			val32 &= ~(1 << 23);
			val32 &= ~(1 << 7);	
		}
		val32 |= (1 << 24);	
	}
	wReg32(0x2ee04, val32);
	nmi_debug(N_VERB, "0x2ee04 (%08x)\n", rReg32(0x2ee04));
	
	if (std != nFM_RADIO) {
		if (ISNM600(pp->chipid) || ISNM601A(pp->chipid))
			upllshift =  (uint32_t)((double)atv_standard_params[std].frontend_ddfs/(1 + pllshift * 0.01));
		else
			upllshift = (uint32_t)((double)atv_standard_params[std].frontend_ddfs_601b0/(1 + pllshift * 0.01));
		val32 = rReg32(0x2ef10);
		val32 &= ~(0x7ffff);
		val32 |= (upllshift & 0x7ffff);
		wReg32(0x2ef10, val32);
	}

	return;
}

static double atv_fe_agc_gain(void)
{
	uint32_t val32, gain;
	double dagc;
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	val32 = rReg32(0x2ee04);
	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		if ((val32 >> 3) & 0x1) {		
			gain = rReg32(0x2eed4)/8;
		} else {
			gain = rReg32(0x2ee98);
		}
	} else {
		if ((val32 >> 22) & 0x1) {		
			gain = rReg32(0x2eed4)/8;
		} else {
			gain = rReg32(0x2ee98);
		}
	}

	dagc = (double)gain/(1 << 12);

	return dagc; 
}

static int atv_fe_agc_lock(void)
{
	uint32_t val32;
	int lock;
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	val32 = rReg32(0x2ee04);
	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		if ((val32 >> 3) & 0x1) {		
			val32 = rReg32(0x2eecc);
		} else {	
			val32 = rReg32(0x2ee90);
		}	
	} else {
		if ((val32 >> 22) & 0x1) {		
			val32 = rReg32(0x2eecc);
		} else {	
			val32 = rReg32(0x2ee90);
		}
	}

	val32 &= 0x3;
	if ((val32 == 0) || (val32 == 2))
		lock = 0;
	else
		lock = 1;

	return lock;
}

static int atv_fe_lock(void)
{
	uint32_t val32;
	int lock = 0;

	val32 = rReg32(0x2ef38);
	lock = (int)(val32 & 0x3);
	if (lock != 1)
		lock = 0;

	return lock;
}

static int atv_fe_get_rssi()
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32; 
	double rssi, agc;
	tAtvRFGain rfg;

	val32 = rReg32(0x2ee98);
	val32 &= 0x3fffff;
	agc = (double)val32/(1 << 12);
	rssi = 20 * log10(agc);

	atv_rf_agc_gain(&rfg);
	rssi += rfg.lnadb;
	rssi += rfg.bblidb;
	rssi +=6;					

	if (pp->lofrequency < 110000000)
		rssi -= 0;
	else if (pp->lofrequency < 300000000)
		rssi -= 4;
	else
		rssi -= 8;

	return (int)-rssi; 
}

static void atv_fe_invert_spectrum(int inv)
{
	uint32_t val32;

	val32 = rReg32(0x2ef10);
	if (inv) {
		val32 |= (1 << 19) | (1 << 20);
	} else {
		val32 &= ~((1 << 19) | (1 << 20));
	}
	wReg32(0x2ef10, val32);
}

static void atv_fe_use_local_agc(tNmiTvStandard std)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;

	if ((std == nNTSC) || (std == nPAL_M) || (std == nPAL_N)) {
		if (pp->localpeak) {

			wReg32(0x2f008, 0x106aa4);

			if (pp->doscan) {
				wReg32(0x2ee9c, 0);
				if (!ISNM601B(pp->chipid)) 
					wReg32(0x2ef18, 0x2d8c4251);
				else
					wReg32(0x2ef18, 0xd8c4251);
			} else {
				wReg32(0x2ee9c, 1);
				if (!ISNM601B(pp->chipid)) 
					wReg32(0x2ef18, 0x2d8c4210);
				else
					wReg32(0x2ef18, 0xd8c4210);
			}

			wReg32(0x300a4, 0x33c80fa0);
			wReg32(0x300a8, 0x11962710);
			wReg32(0x300ac, 0x113c5092);
			wReg32(0x300b0, 0x500b1df);

		} else {
			wReg32(0x2ee9c, 0); 

			wReg32(0x300a4, 0x33c80fa0);
			wReg32(0x300a8, 0x11962710);
			wReg32(0x300ac, 0x5647530);
			wReg32(0x300b0, 0x66007d00);
		}
	} else {
		wReg32(0x2f008, 0x1006c3);
		wReg32(0x2ee9c, 0);
		if (!ISNM601B(pp->chipid))
			wReg32(0x2ef18, 0x2d8c4251);
		else
			wReg32(0x2ef18, 0xd8c4251);
 	}
}

static void atv_fe_set_agc_peak(int en)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	if (en) {
		pp->localpeak = 1;
	} else {
		pp->localpeak = 0;
	}
}

static void atv_vdec_init(void)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	if (ISNM600(pp->chipid))
		val32 = 0x5 | (0xff << 12);
	else
		val32 = 0xc005;
	wReg32(0x2ff9c, val32);

	wReg32(0x30008, 0x3f1004);

	if (ISNM600(pp->chipid)) {
		val32 = 0x1e | (0xa << 12);
		pp->lumaOff = 0xa;	
		wReg32(0x2fef0, val32);
		val32 = UGAIN;
		wReg32(0x2fef4, val32);
		val32 = VGAIN; 
		wReg32(0x2fef8, val32);
	} else {
		wReg32(0x2fef0, 0xcdd078);
		pp->lumaOff = 0xcdd;	
		val32 = UGAIN;
		wReg32(0x2fef4, val32);
		val32 = VGAIN; 
		wReg32(0x2fef8, val32);
	}
	pp->u_gain = UGAIN;
	pp->v_gain = VGAIN;
	pp->uscale = USCALE;
	pp->vscale = VSCALE;
	pp->uvSnrH = UVSNRH;
	pp->uvSnrL = UVSNRL;

	
	if (ISNM600(pp->chipid)) {
		wReg32(0x2fe54, 0x13700a);
	}

	wReg32(0x2fe18, 0x690);

	wReg32(0x2fe14, 0x4400601);

    wReg32(0x2fe00, 0x3110);

	wReg32(0x2ffd0, 0xf);
	
    wReg32(0x2fe34, 0x30003ff6);

	wReg32(0x2fe38, 0x56);
	
	wReg32(0x2fe3c, 0xa0010);
	 
	wReg32(0x30064, 0x7050f1);
	
	wReg32(0x3004c, 0xaaa);

	wReg32(0x30040, 0x1f0425d6);

	wReg32(0x2ffc0, 0x700bb402);

	wReg32(0x2fe00, 0x3110);	

	wReg32(0x2ff6c, 0xf053603f);

	if (ISNM601B(pp->chipid)) {
		val32 = rReg32(0x2ff8c);
		val32 &= ~0x3f;
		wReg32(0x2ff8c, val32);

		val32 = rReg32(0x3005c);
		val32 &= ~(0x3 << 21);
		wReg32(0x3005c, val32);
	}

	if (ISNM600(pp->chipid)) {
		wReg32(0x3009c, 0x215);
		if (chip.inp.uvstep) {
			wReg32(0x300a4, 0x40516580);
			wReg32(0x300a8, 0xb2833);
			wReg32(0x300ac, 0x516580a1);
			wReg32(0x300b0, 0x93340);
		} else {
			wReg32(0x300a4, 0xffffff60);
			wReg32(0x300a8, 0xffffff);
			wReg32(0x300ac, 0xffffff60);
			wReg32(0x300b0, 0xffffff);
		}

		wReg32(0x2fee8, 0x2002);		
	} else {
		val32 = rReg32(0x3009c);
		val32 &= ~0x7;
		val32 |= 0x7;
		wReg32(0x3009c, val32);		
	}

	if (ISNM600(pp->chipid)) {
		wReg32(0x2ff64, 0x2000);
	} else {
		if (ISNM601B(pp->chipid))
			wReg32(0x2ff64, 0xa007ff6);
		else	
			wReg32(0x2ff64, 0xa007ff2);

		wReg32(0x302d8, 0x800);	
#if defined(SYNCMODE)
		atv_vdec_set_hsync_mode(0);
#endif
	}

	return;
}

static void atv_vdec_config_secam()
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	uint32_t option = 1;

	if (ISNM601(pp->chipid)) {
		val32 = rReg32(0x2ff64);
		val32 &= ~(0xff << 24);
		val32 |= (0xb << 24);
		wReg32(0x2ff64, val32); 
		wReg32(0x2fe48, 0x10133013);	
		wReg32(0x2fe54, 0x11131006);
		wReg32(0x2fefc, 0x47ebd);

#if 0
		wReg32(0x300a4, 0x33c80fa0);
		wReg32(0x300a8, 0x11962710);
		wReg32(0x300ac, 0x9645208);
		wReg32(0x300b0, 0xcd0055f0);
#endif
	}

		
	val32 = rReg32(0x2fe00);
	val32 &= ~(0x7 << 4);
	val32 |= (2 << 4);
	wReg32(0x2fe00, val32);	

	wReg32(0x2fe24, 0x5212f684);	

	val32 = rReg32(0x2fe4c);
	val32 &= ~(0xff << 8);
	val32 |= (142 << 8);
	wReg32(0x2fe4c, val32);	

	if (ISNM600C0(pp->chipid))
		wReg32(0x30238, 0x1);

	wReg32(0x30290, 0x10);
	wReg32(0x30250, 0x47fb60);
	wReg32(0x30244, 0x300a372);

	wReg32(0x2fef0, 0xd9006f);
	pp->lumaOff = 0xd90;
	wReg32(0x2fef4, SECAM_UGAIN);
	wReg32(0x2fef8, SECAM_VGAIN);
	pp->u_gain = SECAM_UGAIN;
	pp->v_gain = SECAM_VGAIN;
	pp->uscale = SECAM_USCALE;
	pp->vscaleSecam = SECAM_VSCALE;

	wReg32(0x30248, 0x240c);
	wReg32(0x30240, 0x14005240);

	val32 = rReg32(0x2ffdc);
	val32 |= (1 << 1);
	wReg32(0x2ffdc, val32);

	val32=0x20000a;
	wReg32(0x2ffec, val32);

	val32 = 0x1108169f;
	wReg32(0x2fe18, val32);

	val32 = 0x4401801;
	wReg32(0x2fe14, val32);

	val32 = rReg32(0x2ffc0);
	val32 |= 0x400;
	wReg32(0x2ffc0, val32);
	wReg32(0x2ffd4, 0x80048120);
	wReg32(0x30040, 0x7f0a25d6);
	 
	if (option==1) {
		val32 = 0xd8a52c52;
		wReg32(0x30288, val32);
		val32 = 0x7b09;
		wReg32(0x3028c, val32);
	} else if (option==2) {
		val32=0xdbd42dea;
		wReg32(0x30288, val32);
		val32=0x7642;
		wReg32(0x3028c, val32);
   } else if (option==3) {
      val32 = 0xdee52f72;
      wReg32(0x30288, val32);
      val32 = 0x71a9;
      wReg32(0x3028c, val32);
   } else {
		val32 = 0xe1d930ec;
		wReg32(0x30288, val32);
		val32 = 0x6d3b;
		wReg32(0x3028c, val32);
   }
}

static void atv_vdec_config(tNmiTvStandard std)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	uint32_t serr_array_idx;
	tNmiTvStandard sstd;
	int isSecam = 1;

	sstd = std;
	if (std == nSECAM_B) {
		std = nPAL_B;
	} else if (std == nSECAM_D) {
		std = nPAL_D;
	} else if (std == nSECAM_G) {
		std = nPAL_G;
	} else if (std == nSECAM_H) {
		std = nPAL_H;
	} else if (std == nSECAM_K) {
		std = nPAL_K;
	} else if (std == nSECAM_K1) {
		std = nPAL_K1;
	} else {
		isSecam = 0;
	}

	if (ISNM600C0(pp->chipid)) {
		val32 = rReg32(0x3009c);
		if (isSecam) {
			val32 &= ~(1 << 1);
		} else {
			val32 |= (1 << 1);
		}
		wReg32(0x3009c, val32);
	}

	if (ISNM601(pp->chipid)) {
		int ix = chip.inp.size;

		if (!isSecam) {
			val32 = 0x3050007;
			if (std == nNTSC)
				val32 |= (0xa1 << 4);
			else
				val32 |= (0xd1 << 4);
			wReg32(0x302d4, val32);
		}

		if ((szpar[ix].width >= 320) && (szpar[ix].height >= 240)) {
			wReg32(0x30238, 0x31);
		} else if ((szpar[ix].width >= 220) && (szpar[ix].height >= 176)) {
			wReg32(0x30238, 0x21);
		} else if ((szpar[ix].width >= 176) && (szpar[ix].height >= 144)) {
			wReg32(0x30238, 0x11);
		} else {
			wReg32(0x30238, 0x01);
		}

		if (!((std == nNTSC) || (std == nPAL_M) || (std == nPAL_N))) {
			wReg32(0x300a4, 0x33c80fa0);
			wReg32(0x300a8, 0x11962710);
			wReg32(0x300ac, 0x546a410);
			wReg32(0x300b0, 0x600ffff);
		}
	}

	if (ISNM601(pp->chipid)) {
		val32 = (atv_standard_params[std].odd_field_f1 << 24) |
 						(atv_standard_params[std].odd_field_l1 << 25) |
						0x137006;  
		wReg32(0x2fe54, val32);
	} 
	
	val32 = rReg32(0x2fe00);
	val32 &= ~0x1;  
	val32 &= ~(0x7 << 4);
	val32 &= ~(0x3 << 8);
	val32 |= (1 << 8) | ((atv_standard_params[std].color_standard & 0x7) << 4);
	wReg32(0x2fe00, val32);	

	wReg32(0x2fe24, atv_standard_params[std].blo_fsc_seed);

	if (ISNM600(pp->chipid)) {
		val32 =	((atv_standard_params[std].first_active_line & 0x3ff) << 0)
						|((atv_standard_params[std].last_active_line & 0x3ff)  << 12);
	} else {
		val32 =	((atv_standard_params[std].first_active_line_n & 0x3ff) << 0)
						|((atv_standard_params[std].last_active_line_n & 0x3ff)  << 12);
		if ((std == nNTSC) || (std == nPAL_M))
			val32 |= (1 << 24);
	}
	wReg32(0x2fe48, val32);	

	val32 =	((atv_standard_params[std].hsync_to_burst_start & 0xff) << 0)
					|((atv_standard_params[std].hsync_to_burst_end & 0xff)  << 8);
	wReg32(0x2fe4c, val32);	

	val32 = rReg32(0x2fe50);
  	val32 &= ~0x3ff;
	val32 &= ~(0x3ff << 12);
	val32 |= ((atv_standard_params[std].active_video_start & 0x3ff) << 0)
				|((atv_standard_params[std].active_video_end & 0x3ff)  << 12);
	wReg32(0x2fe50, val32);	

	if (ISNM600(pp->chipid)) {
		val32 = (atv_standard_params[std].first_active_line & 0x3ff)
						| ((atv_standard_params[std].last_active_line & 0x3ff)  << 12);
	} else {
		val32 = (atv_standard_params[std].first_active_line_n & 0x3ff)
						| ((atv_standard_params[std].last_active_line_n & 0x3ff)  << 12);
	}
	if (chip.inp.interlace) {
		val32 &= ~(0x1 << 25);
		val32 |= (1 << 24);	
	} else {
		if ((std == 0) || (std == 2)) { 
			val32 |= (1 << 25);
		} else {
			val32 &= ~(1 << 25);
		}
	}
	wReg32(0x30090, val32);

	val32 = (atv_standard_params[std].active_video_start & 0x3ff)
					|((atv_standard_params[std].active_video_end & 0x3ff)  << 12);
	wReg32(0x30094, val32);	

	wReg32(0x2fe80, atv_standard_params[std].backporch_value);

	val32 = rReg32(0x2fe04);
	val32 |= (1 << 0);
	wReg32(0x2fe04, val32);

	val32 = rReg32(0x2ff6c);
	val32 &= ~(0x3ff << 8);
	val32 |= (atv_standard_params[std].line_duration & 0x3ff) << 8;
	wReg32(0x2ff6c, val32);

	if (ISNM600(pp->chipid)) {
		serr_array_idx = atv_standard_params[std].serr_line_index;
		
		val32 = ((atv_standard_params[std].mun_serr_lines & 0x1f) << 4)
						|((serration_line_indeces[serr_array_idx][0] & 0x3ff)  << 12)
						|((serration_line_indeces[serr_array_idx][1] & 0x3ff)  << 22);
		wReg32(0x2ff40, val32);	
		
		val32 =	((serration_line_indeces[serr_array_idx][2] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][3] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][4] & 0x3ff)  << 20);
		wReg32(0x2ff44, val32);	

		val32 =	((serration_line_indeces[serr_array_idx][5] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][6] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][7] & 0x3ff)  << 20);
		wReg32(0x2ff48, val32);	

		val32 =	((serration_line_indeces[serr_array_idx][8] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][9] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][10] & 0x3ff)  << 20);
		wReg32(0x2ff4c, val32);	

		val32 =	((serration_line_indeces[serr_array_idx][11] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][12] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][13] & 0x3ff)  << 20);
		wReg32(0x2ff50, val32);
		
		val32 =	((serration_line_indeces[serr_array_idx][14] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][15] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][16] & 0x3ff)  << 20);
		wReg32(0x2ff54, val32);	

		val32 =	((serration_line_indeces[serr_array_idx][17] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][18] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][19] & 0x3ff)  << 20);
		wReg32(0x2ff58, val32);	

		val32 =	((serration_line_indeces[serr_array_idx][20] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][21] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][22] & 0x3ff)  << 20);
		wReg32(0x2ff5c, val32);	

		val32 =	((serration_line_indeces[serr_array_idx][23] & 0x3ff)  << 0)
				|	((serration_line_indeces[serr_array_idx][24] & 0x3ff)  << 10)
				|	((serration_line_indeces[serr_array_idx][25] & 0x3ff)  << 20);
		wReg32(0x2ff60, val32);
	} else {
		wReg32(0x2ff40, (atv_standard_params[std].serr_line_end1 << 16));
		wReg32(0x2ff44, (atv_standard_params[std].serr_line_start2 | (atv_standard_params[std].serr_line_end2 << 16)));
	}

	val32 = rReg32(0x2ff70);
	val32 &= ~0xffff;
	val32 |= (atv_standard_params[std].inv_linewidth & 0xffff);
	wReg32(0x2ff70, val32);

	val32 = rReg32(0x2ff78);
	val32 &= ~((1 << 24) | (1 << 25) | (1 << 26));
	val32 |= (atv_standard_params[std].num_vsync_pulses & 0x7) << 24;
	wReg32(0x2ff78, val32);
	
	val32 = rReg32(0x2ff7c);
	val32 &= ~0x3ff;
	val32 &= ~(0x1ff << 12);
	val32 |=	((atv_standard_params[std].num_lines_per_frame & 0x3ff) << 0)
			 |	((atv_standard_params[std].num_lines_per_field & 0x1ff) << 12);
	val32 &= ~(0x3 << 24);
	wReg32(0x2ff7c, val32);
	nmi_debug(N_VERB, "Vdec: 0x2ff7c (%08x)\n", rReg32(0x2ff7c));

	val32 = rReg32(0x30000);
	val32 &= ~(0x7ff << 8);
	val32 |=	((atv_standard_params[std].tx_sig_mean & 0x7ff) << 8);		
	wReg32(0x30000, val32);

	val32 =	((atv_standard_params[std].useful_duration & 0x3ff) << 0);		
	wReg32(0x30060, val32);

	val32 = (atv_standard_params[std].active_vid_hcnt_max) |
			  		(atv_standard_params[std].active_vid_vcnt_max << 10);
	wReg32(0x2fefc, val32);

	if (ISNM600(pp->chipid)) {
		val32 = atv_standard_params[std].remod_blank_level;
		wReg32(0x2feec, val32);
	}

	if (std != sstd) 
		atv_vdec_config_secam();
	
#if 0
	atv_vdec_doppler_enhancement();
#endif

	return;
}

static void atv_vdec_track_doppler(void)
{
#if defined(SYNCMODE)
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	double d_lock;
	double bloAvgAlpha = 0.2;
	double d_bloAvg    = pp->bloAvg;
	int    decre;
	static int prev_decre = 0;
	double decre_diff;
	double decre_alpha = 0.3;

	if (ISNM600(pp->chipid))
		return;

	val32 = rReg32(0x2fe40) & 0x3;	
	d_lock = val32 == 3 ? -1.0 : (double)val32;
	pp->bloAvg = (d_lock - d_bloAvg)*bloAvgAlpha + d_bloAvg;

	
	if (ISNM601A(pp->chipid))
		val32 = rReg32(0x3000c) >> 16;	
	else
		val32 = rReg32(0x30018) & 0x7ffff;
	pp->i_nv = val32;

	
	decre = (int)rReg32(0x2fe1c);
	decre &= 0x3fffff;
	if (decre >= (1 << 21))
		decre -= (1 << 22);

	pp->decreDiff = prev_decre - decre;
	decre_diff = (double)(pp->decreDiff);
	prev_decre = decre;
	pp->decreAvg = (decre_diff - pp->decreAvg)*decre_alpha + pp->decreAvg;


	{
		int	   i_nv     = pp->i_nv;
		int    i_nv_thr1 = 6522;			
		int    i_nv_thr0 = 20626;			
		double d_bloAvg_thr1 =  0.3;	
		double d_bloAvg_thr0 = -0.7;	
		double d_decreAvg = pp->decreAvg;
		double abs_decAvg = d_decreAvg < 0 ? -d_decreAvg : d_decreAvg;
		int    i_decdiff_thr1 = 500;	
		int    i_decdiff_thr0 = 10000;	


		if (pp->standard != nFM_RADIO &&
			pp->standard != nSECAM_B &&
			pp->standard != nSECAM_D &&
			pp->standard != nSECAM_G &&
			pp->standard != nSECAM_H &&
			pp->standard != nSECAM_K &&
			pp->standard != nSECAM_K1	) {

			if (ISNM601(pp->chipid)) {
				if (pp->hsyncMode == 0) {
					if ((pp->hsyncCount >= 20) && (d_bloAvg > d_bloAvg_thr1) && (i_nv < i_nv_thr1) && (abs_decAvg < i_decdiff_thr1)) {
						atv_vdec_set_hsync_mode(1);
					} 
					else if ((d_bloAvg < d_bloAvg_thr0) || (i_nv > i_nv_thr0) || (abs_decAvg > i_decdiff_thr0)) {
						pp->hsyncCount = 0;
					}
					else {
						pp->hsyncCount++;
					}

				} else { 
					if ((d_bloAvg < d_bloAvg_thr0) || (i_nv > i_nv_thr0) || (abs_decAvg > i_decdiff_thr0)) {
						atv_vdec_set_hsync_mode(0);
						pp->hsyncCount = 0;
					}
				}
			}
		}
	}
#endif
}


static void atv_vdec_sync_status(tAtvVdecSyncStatus *p)
{
	uint32_t	val32, tmp;

	val32 = rReg32(0x2ff90);
	tmp = val32 & 0x7ff;
	p->hcorr = (int)(tmp - (2 * (tmp & 0x400))); 
	tmp = (val32 >> 12) & 0x7ff;
	p->vcorr =(int)(tmp - (2 * (tmp & 0x400))); 
	p->vsynclock =  (val32 >> 24) & 0x3;

	val32 = rReg32(0x2fe44);
	val32 &= 0x1ff;
	if (val32 >= 256) {
		p->colorlocklevel = (int)(val32 - 512);
	} else {
		p->colorlocklevel = (int)val32;
	}

}

static int atv_vdec_blo_lock(void)
{
	uint32_t val32;
	int lock;

	val32 = rReg32(0x2fe00);
	val32 >>= 4;
	val32 &= 0x7;
	if (val32 <= 1) {
		val32 = rReg32(0x2fe40);
		lock = (val32 & 0x3);
	} else {
		val32 = rReg32(0x30248);
		val32 &= (1 << 28);
		if (val32 > 0)
			lock = 1;
		else
			lock =3;
	}

	return lock;
}

static uint32_t atv_vdec_get_avg_line_width_dev(void)
{
	uint32_t val32;

	val32 = rReg32(0x2ffe4);
	val32 = (val32 >> 8) & 0x3ff;

	return val32;
}

static uint32_t atv_vdec_get_snr(void)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t nv;

	nv = rReg32(0x3000c);
	if (ISNM601B(pp->chipid))
		nv &= 0x7ffff;
	else
		nv &= 0xffff;

	return nv;
}

static double atv_vdec_get_noise_var(void)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	double cn;
	uint32_t nv;

	nv = rReg32(0x3000c);
	if (ISNM601B(pp->chipid))
		nv &= 0x7ffff;
	else
		nv &= 0xffff;

	if (nv == 0) {
		uint32_t val32;

		val32 = rReg32(0x2ff90);
		if (((val32 >> 24) & 0x3) == 2)
			cn = 45.;
		else
			cn = -6.;
	} else 
		cn =  (10.0 * log10(((double)(1 << 15))/((double)(2 * nv))));

	return cn;
}

static void atv_vdec_set_uv_gain(int step)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32, ugain, vgain;
	double gainstep;
	
	nmi_debug(N_UV, "UV: step (%d)\n", step);
	
	if ((step > -6) && (step < 6)) {
		gainstep = ((step + 5)*0.1) - 0.5;
		
		if ((pp->standard == nSECAM_B) || 
			(pp->standard == nSECAM_D) ||
			(pp->standard == nSECAM_G) ||
			(pp->standard == nSECAM_H) ||
			(pp->standard == nSECAM_K) ||
			(pp->standard == nSECAM_K1)) {
			int vg;
			pp->uscale = ugain = (uint32_t)((1 + gainstep) * SECAM_USCALE);
			pp->vscaleSecam = vg = (int)((1 + gainstep) * SECAM_VSCALE);	
			vgain = 1024 + vg;
		} else {
			pp->uscale = ugain = (uint32_t)((1 + gainstep) * USCALE);
			pp->vscale = vgain = (uint32_t)((1 + gainstep) * VSCALE);
		}
		nmi_debug(N_UV, "gain step (%f), ugain (%d), vgain (%d)\n", gainstep, ugain, vgain);

		val32 = rReg32(0x2fef4);
		val32 &= ~0x3ff;
		val32 |= ugain;
		wReg32(0x2fef4, val32);
		
		val32 = rReg32(0x2fef8);
		val32 &= ~0x3ff;
		val32 |= vgain;
		wReg32(0x2fef8, val32);
		
		pp->u_gain = ugain;
		pp->v_gain = vgain;
	}	 

	return;
}

static void atv_vdec_set_uv_snr_threshold(double snrH, double snrL)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;

	nmi_debug(N_UV, "UV Snr Threshold: high (%f), low (%f)\n", snrH, snrL);
	pp->uvSnrH = snrH;
	pp->uvSnrL = snrL;
}

static void atv_vdec_set_uv_avg_factor(double factor)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;

	if ((factor >= 0.) && (factor <= 1.)) {
		pp->uvfact = factor;
	}
	
	return;

}

static void atv_vdec_set_min_scale(double scale)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	val32 = rReg32(0x3005c);
	val32 &= ~0x1;
	wReg32(0x3005c, val32);
	
	val32 = rReg32(0x3009c);
	val32 &= ~0x1;
	wReg32(0x3009c, val32);

	pp->minScale = scale;
	return;
}

static void atv_vdec_adj_uv_gain(void)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	double snr, scale;
	double snrH = pp->uvSnrH;
	double snrL = pp->uvSnrL;
	uint32_t val32, uscale, vscale;
	uint32_t ouscale = pp->uscale;
	uint32_t ovscale = pp->vscale;
	int ovscaleSecam = pp->vscaleSecam;
	double factor = pp->uvfact;  
	double  agcFe;
	int cplllock;
	uint8_t temptuner;

	if (ISNM601B(pp->chipid)){
		agcFe = atv_fe_agc_gain();
		if (agcFe > 900){
			cplllock = atv_fe_lock();

			if (!cplllock){
				temptuner = atvrfread(0x08);
				temptuner &= 0x7f;
				atvrfwrite(0x8, temptuner);
				temptuner |= 0x80;
				atvrfwrite(0x8, temptuner);

				wReg32(0x2ec04, 0x1);
				wReg32(0x2ec04, 0x0);

			}
		}
	}

	if (chip.inp.uvstep || ISNM601(pp->chipid))
		return;

	snr = atv_vdec_get_noise_var();
	if (snr >= snrH) {
		scale = 1.;
	} else if (snr <= snrL) {
		scale = 0.;
	} else {
		scale = (snr - snrL)/(snrH - snrL);
	}

	if (scale <= pp->minScale) {
		scale = pp->minScale;
	}

	nmi_debug(N_UV, "snr (%f), scale (%f)\n", snr, scale);

	if ((pp->standard == nSECAM_B) || 
		(pp->standard == nSECAM_D) ||
		(pp->standard == nSECAM_G) ||
		(pp->standard == nSECAM_H) ||
		(pp->standard == nSECAM_K) ||
		(pp->standard == nSECAM_K1)) {
		int vg;

		uscale = (uint32_t)(pp->u_gain * scale);

		uscale = (uint32_t)((ouscale * (1 - factor)) + (factor * uscale));
		pp->uscale = uscale;

		vg = (int)pp->v_gain - 1024;
		vg = (int)((ovscaleSecam * (1 - factor)) + (factor * vg));
		pp->vscaleSecam = vg;

		vscale = (uint32_t)(1024 + (vg * scale));

	} else {
		uscale = (uint32_t)(pp->u_gain * scale);

		uscale = (uint32_t)((ouscale * (1 - factor)) + (factor * uscale));
		pp->uscale = uscale;

		vscale = (uint32_t)(pp->v_gain * scale);

		vscale = (uint32_t)((ovscale * (1 - factor)) + (factor * vscale));
		pp->vscale = vscale;


	}
	nmi_debug(N_UV, "uscale (%d), vscale (%d)\n", uscale, vscale);
	
	val32 = rReg32(0x2fef4);
	val32 &= ~0x3ff;
	val32 |= uscale;
	wReg32(0x2fef4, val32);

	val32 = rReg32(0x2fef8);
	val32 &= ~0x3ff;
	val32 |= vscale;
	wReg32(0x2fef8, val32);

	return;
}

static void atv_vdec_set_luma_gain(int gain)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	uint32_t step_size=3;
	int luma_gain;
	uint32_t luma_gain_unsigned;

	val32 = rReg32(0x2fef0);
	val32 &= ~(0x3ff);
	if (ISNM600(pp->chipid)) {
		if ((pp->standard == nSECAM_B) ||
			 (pp->standard == nSECAM_D) ||
			 (pp->standard == nSECAM_G) ||
			 (pp->standard == nSECAM_H) ||
			 (pp->standard == nSECAM_K) || (pp->standard == nSECAM_K1)) {
			 luma_gain=111+(gain-4)*step_size*4;			
		} else {
			 luma_gain=30+(gain-4)*step_size;
		}
	} else {
		if ((pp->standard == nSECAM_B) ||
			 (pp->standard == nSECAM_D) ||
			 (pp->standard == nSECAM_G) ||
			 (pp->standard == nSECAM_H) ||
			 (pp->standard == nSECAM_K) || (pp->standard == nSECAM_K1)) {
			 luma_gain=111+(gain-4)*step_size*4;	
		} else{			
			luma_gain=120+(gain-4)*step_size*4;	
	}
	}
	luma_gain=(luma_gain<0)?0:luma_gain;
	luma_gain=(luma_gain>=1024)?1023:luma_gain;
	luma_gain_unsigned=	(uint32_t)luma_gain;
	val32 |=(luma_gain_unsigned & 0x3ff);
	wReg32(0x2fef0, val32);
}

static void atv_vdec_set_luma_offset(int off)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	uint32_t doffset = pp->lumaOff;
	int noff, doff = (int)doffset;

	nmi_debug(N_VERB, "luma offset (%d)\n", off);

	if (doff >= 2048)
		doff -= 4096;

	nmi_debug(N_VERB, "default luma offset (%d)\n", doff);

	noff = doff + (off * LUMA_OFFSET_STEP);

	nmi_debug(N_VERB, "new luma offset (%d)\n", noff);

	if (noff >= 2048)
		noff=2047;
	else if (noff <= -2048)
		noff=-2048;

	if (noff < 0)
		noff += 4096;

	nmi_debug(N_VERB, "final new luma offset (%d)\n", noff);

	val32 = rReg32(0x2fef0);
	val32 &= ~(0xfff << 12);
	val32 |= ((noff & 0xfff) << 12);
	wReg32(0x2fef0, val32);

	return;
}

static void atv_vdec_set_hsync_mode(int set_hist)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	if (set_hist == 1) {
		wReg32(0x2ff74, 0x75747b09);	
		wReg32(0x2ffa4, 0xa00a);		
		if (pp->standard == nNTSC) {
			wReg32(0x2ffa8, 0x40000a);		
		} else {
			wReg32(0x2ffa8, 0x100000a);		
		}
		wReg32(0x2ffac, 0x3e83f);		
		pp->hsyncMode = 1;
	} else if (pp->hsyncMode == 1) {
		if (set_hist != 2) {
			wReg32(0x2ec04, 0xa);
		}
		wReg32(0x30020, pp->i_nv);		
		wReg32(0x2ffa8, 0xa000a);		
		wReg32(0x2ff74, 0x25747b09);	
		wReg32(0x2ffa4, 0xf030);		
		wReg32(0x2ffac, 0x3e842);		
		if (set_hist != 2) {
			wReg32(0x2ec04, 0x8);
			nmi_delay(300);
			wReg32(0x2ec04, 0x0);
		}
#if defined(PC_ENV)
		atv_fpga_play();
#endif
		pp->hsyncCount = 0;
		pp->hsyncMode = 0;
	}
}

static void atv_vdec_set_ch_switch_speed(int fast)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	if (ISNM601(pp->chipid)) {
		if (fast) {
			wReg32(0x2ff80, 0x921053f);
		} else {
			wReg32(0x2ff80, 0x9210505);
		}		
	}
}

static void atv_vdec_doppler_enhancement(void)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	
	if (ISNM601B(pp->chipid)) {
		uint32_t val32;

		val32 = rReg32(0x2ff70);
		val32 |= (1 << 16);
		wReg32(0x2ff70, val32);
		
		val32 = rReg32(0x2ff74);
		val32 &= ~(0x7 << 28);
		val32 |= (1 << 28);
		wReg32(0x2ff74, val32);

		val32 = rReg32(0x2ffa8);
		val32 &= ~(0xffff << 16);
		val32 |= (0x1a << 16);
		wReg32(0x2ffa8, val32);
		
		val32 = rReg32(0x2ffec);
		val32 &= ~0xff;
		val32 |= 0x1a;
		val32 |= (1 << 25);
		wReg32(0x2ffec, val32);

		val32 = rReg32(0x3030c);
		val32 &= ~(0xf << 20);
		val32 |= (4 << 20);
		val32 |= 0x1;
		wReg32(0x3030c, val32);
	}
}


static void atv_be_line_delay(int delay)
{
	uint32_t val32;  

	if (chip.inp.btrate == nClk13_5) {
		if (delay < 32)
			delay = 32;
	} else if (chip.inp.btrate == nClk27) {
		if (delay < 240)
			delay = 240;				
	} else
		return;

	val32 = rReg32(0x31290);
	val32 &= ~(0x1ff << 12);
	val32 |= ((delay & 0x1ff) << 12);
	wReg32(0x31290, val32);
}

static void atv_be_init(void)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	int ix = chip.inp.size;
	
	val32 = (8 << 0) |
			  		(6 << 10) |
			  		(16 << 20) | 
			  		(16 << 25) |
					(1 << 30) |
			  		(1 << 31);
	wReg32(0x31288, val32);

	wReg32(0x3125c, 0x28ffff);

	if (ISNM600C0(pp->chipid))
		wReg32(0x3128c, 0x40120);
	else
		wReg32(0x3128c, 0x42020);

	val32 = szpar[ix].hscale_p | 
				(szpar[ix].hscale_r << 4) | 
				((szpar[ix].hscale_d_m1 & 0x3) << 8) | 
				(szpar[ix].hscale_ores_m1 << 10) | 
				(0x3 << 20) |
				(((szpar[ix].hscale_d_m1 >> 2) & 0x1) << 22);
	wReg32(0x31220, val32);
	wReg32(0x31224, szpar[ix].hscale_frac_lut0);
	wReg32(0x31228, szpar[ix].hscale_frac_lut1);

	if (ISNM601(pp->chipid)) {
		if (szpar[ix].height == 180) {
			szpar[ix].vscale_p = 2;
			szpar[ix].vscale_r = 1;
			szpar[ix].vscale_d_m1 = 0;
			szpar[ix].vscale_ores_m1 = 191;
			szpar[ix].vscale_frac_lut0 = 0x80;
			szpar[ix].vscale_frac_lut1 = 0;

			szpar[ix].vscale_p_ntsc = 4;
			szpar[ix].vscale_r_ntsc = 1;
			szpar[ix].vscale_d_m1_ntsc = 0;
			szpar[ix].vscale_ores_m1_ntsc = 191;
			szpar[ix].vscale_frac_lut0_ntsc = 0xc840;
			szpar[ix].vscale_frac_lut1_ntsc = 0;
		}
	}
	val32 = szpar[ix].vscale_p | (szpar[ix].vscale_r << 4) | (szpar[ix].vscale_d_m1 << 8) | (szpar[ix].vscale_ores_m1 << 10) | (0x3 << 20);
	wReg32(0x3122c, val32);
	wReg32(0x31230, szpar[ix].vscale_frac_lut0);
	wReg32(0x31234, szpar[ix].vscale_frac_lut1);

	val32 = szpar[ix].h_trig_thrd | (szpar[ix].v_trig_thrd << 10) | (1 << 19) | (1 << 20);
	if (chip.inp.input_order) {	
		val32 &= ~(1 << 19);
	}
	if (chip.inp.btformat == nBTRGB) {
		val32 |= (1 << 23);			
		val32 &= ~(1 << 20);		
		val32 &= ~(1 << 19);		
	}
	if (ISNM601B(pp->chipid)) {
		val32 |= (1 << 24);
		val32 &= ~(1 << 25);
	}
	wReg32(0x31298, val32);

	if (ISNM600(pp->chipid))
		wReg32(0x31290, szpar[ix].fb_cfg);
	else {
		double hr = (double)szpar[ix].width;
		double ratio;
		uint32_t thrd;

		val32 = rReg32(0x31290);
		if (chip.inp.half_frame_rate) {
			val32 &= ~(0xfff);
			val32 |= 0x222;
		}
		val32 &= ~(0x1ff << 12);
		switch (chip.inp.btrate) {
		case nClk6_75:
			ratio = 4.0;
			break;
		case nClk13_5:
			ratio = 2.0;
			break;
		case nClk27:
		default:
			ratio = 1.0;
			break;
		}
		if ((704./hr) < ratio)
			thrd = 32;
		else if (((hr - 16.)*2) > 480)
			thrd = 480;
		else 
			thrd = (uint32_t)((hr - 16) * 2);
		val32 |= (thrd << 12);
		wReg32(0x31290, val32);
	} 

	atv_be_set_mode(nAuto);

	val32 = (1 << 0) |   
				 (1 << 1) |
				(1 << 2) |
				(1 << 6) | 
				(1 << 7) | 
				(1 << 8) |
				(1 << 9) |
				(1 << 10) |
				(1 << 11) |
				(1 << 12) |
				(1 << 15);
	if (ISNM600C0(pp->chipid)) {
		val32 |= (1 << 14);
	}
	if (chip.inp.interlace) {
		if (ISNM600(pp->chipid)) {
			val32 &= ~(1 << 6);
		}
	}
	if (ISNM601(pp->chipid)) {
		val32 &= ~(1 << 12);	
		val32 |= (1 << 18);		

		if ((szpar[ix].width%16) || (szpar[ix].height%16)) {
			val32 |= (1 << 20);
			if (szpar[ix].width == 220) {
				wReg32(0x312b4, ((229 << 16) | 10));
			} else 
				wReg32(0x312b4, ((szpar[ix].width - 1) << 16));
			if (szpar[ix].height == 180) {
				wReg32(0x312b8, ((185 << 16) | 6));
			} else if (szpar[ix].height == 60) {
				wReg32(0x312b8, ((61 << 16) | 2));
			} else 
				wReg32(0x312b8, ((szpar[ix].height -1) << 16));
		} else
			val32 &= ~(1 << 20);
	}

	if (ISNM600(pp->chipid)) {
		uint32_t val1;

		if ((szpar[ix].width == 426) &&
			(szpar[ix].height == 320)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x4;
			val1 |= (0x3 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (425 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xc840);
			wReg32(0x31308, 0);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x4;
			val1 |= (0x3 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (319 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0xc840);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}

		if ((szpar[ix].width == 480) &&
			(szpar[ix].height == 360)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x3;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (479 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xb50);
			wReg32(0x31308, 0);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x3;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (359 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0xb50);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}

		if ((szpar[ix].width == 480) &&
			(szpar[ix].height == 280)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x3;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (479 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xb50);
			wReg32(0x31308, 0);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x7;
			val1 |= (0x6 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (279 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0xeb97520);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}

		if ((szpar[ix].width == 360) &&
			(szpar[ix].height == 240)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x9;
			val1 |= (0x8 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (359 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xcb975420);
			wReg32(0x31308, 0xe);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x2;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (239 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0x0);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}
	}
	wReg32(0x31204, val32);
	nmi_debug(N_VERB, "0x31204 (%08x)\n", rReg32(0x31204));

	if (ISNM601(pp->chipid)) {
	 	int nsh=8;
		wReg32(0x31328,((0x200)/nsh));
		wReg32(0x3132c,((0x17c)/nsh));
		wReg32(0x31330,((0x11a)/nsh));
		wReg32(0x31334,((0x0b6)/nsh));
		wReg32(0x31338,((0x066)/nsh));
		wReg32(0x3133c,((0x040)/nsh));
		wReg32(0x31340,((0x028)/nsh));
		wReg32(0x31344,((0x3fff)/nsh));
	}

	if (ISNM601B(pp->chipid)) {
		val32 = rReg32(0x31350);	
		val32 |= (0x1 << 26);		
		wReg32(0x31350, val32);

		wReg32(0x3123c, 0x3);
	}

}

static void atv_be_config(tNmiTvStandard std)
{
	uint32_t val32;
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	int ix = chip.inp.size;

	if (std == nSECAM_B) {
		std = nPAL_B;
	} else if (std == nSECAM_D) {
		std = nPAL_D;
	} else if (std == nSECAM_G) {
		std = nPAL_G;
	} else if (std == nSECAM_H) {
		std = nPAL_H;
	} else if (std == nSECAM_K) {
		std = nPAL_K;
	} else if (std == nSECAM_K1) {
		std = nPAL_K1;
	} 

	val32 = rReg32(0x31204);
	val32 &= ~((1 << 3) | (1 << 4) | (1 << 5) | (1 << 13));
	val32 |=	((atv_standard_params[std].repeater_enable & 0x1) << 3)
				|	((atv_standard_params[std].hscale_enable & 0x1) << 4)
				|	((atv_standard_params[std].vscale_enable & 0x1) << 5)
				|	((atv_standard_params[std].serr_line_index & 0x1) << 13);
	if ((std == nNTSC) || (std == nPAL_M)) {
		if (szpar[ix].height < 240)
			val32 |= (1 << 5);
	}

	if (ISNM600(pp->chipid)) {
		uint32_t val1;

		val32 &= ~(1 << 16);

		if ((szpar[ix].width == 426) &&
			(szpar[ix].height == 320)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x4;
			val1 |= (0x3 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (425 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xc840);
			wReg32(0x31308, 0);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x4;
			val1 |= (0x3 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (319 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0xc840);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}

		if ((szpar[ix].width == 480) &&
			(szpar[ix].height == 360)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x3;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (479 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xb50);
			wReg32(0x31308, 0);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x3;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (359 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0xb50);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}

		if ((szpar[ix].width == 480) &&
			(szpar[ix].height == 280)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x3;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (479 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xb50);
			wReg32(0x31308, 0);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x7;
			val1 |= (0x6 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (279 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0xeb97520);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}

		if ((szpar[ix].width == 360) &&
			(szpar[ix].height == 240)) {
			val1 = rReg32(0x31300);
			val1 &= ~(0xff);
			val1 |= 0x9;
			val1 |= (0x8 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (359 << 10);
			wReg32(0x31300, val1);
			wReg32(0x31304, 0xcb975420);
			wReg32(0x31308, 0xe);

			val1 = rReg32(0x3130c);
			val1 &= ~(0xff);
			val1 |= 0x2;
			val1 |= (0x2 << 4);
			val1 &= ~(0x3ff << 10);
			val1 |= (239 << 10);
			wReg32(0x3130c, val1);
			wReg32(0x31310, 0x0);
			wReg32(0x31314, 0x0);

			val32 |= (1 << 16);
		}
	}
	wReg32(0x31204, val32);

	if (ISNM600(pp->chipid)) {
		val32 = ((atv_standard_params[std].hcnt_start & 0x3ff)	<<	0)
				|	((atv_standard_params[std].hcnt_end & 0x3ff)	<<	16);
	} else {
		val32 = ((atv_standard_params[std].hcnt_start_601 & 0x3ff)	<<	0)
				|	((atv_standard_params[std].hcnt_end & 0x3ff)	<<	16);
	}
	wReg32(0x31208, val32);

	val32 = ((atv_standard_params[std].vcnt_start & 0x1ff)	<<	0)
			|	((atv_standard_params[std].vcnt_end & 0x1ff)	<<	16);
	wReg32(0x3120c, val32);

	val32 = ((atv_standard_params[std].vcnt_start & 0x1ff)	 <<	0)
			|	((atv_standard_params[std].vcnt_end & 0x1ff) <<	16);
	wReg32(0x31210, val32);

	if (ISNM600(pp->chipid))
		val32 =	(atv_standard_params[std].rptr_right & 0x7);
	else
		val32 =	(atv_standard_params[std].rptr_right_601 & 0x7);
	val32 |= ((atv_standard_params[std].rptr_left	& 0x7) << 3)
			|	((atv_standard_params[std].rptr_blank & 0x3ff)	<< 10)
			|	((atv_standard_params[std].rptr_evn_top & 0x1) << 6)
			|	((atv_standard_params[std].rptr_evn_bottom & 0x1) << 7)
			|	((atv_standard_params[std].rptr_odd_top & 0x1) << 8)
			|	((atv_standard_params[std].rptr_odd_bottom & 0x1) << 9);
	if (ISNM601(pp->chipid)) {
		if (szpar[ix].width == 220) {
			val32 |= (4 << 20);
		} else {
			val32 &= ~(0xf << 20);
		}
	}
	wReg32(0x31218, val32);

	if (std == nNTSC) {
		val32 = szpar[ix].vscale_p_ntsc | (szpar[ix].vscale_r_ntsc << 4) | (szpar[ix].vscale_d_m1_ntsc << 8) | (szpar[ix].vscale_ores_m1_ntsc << 10);
		wReg32(0x3122c, val32);
		wReg32(0x31230, szpar[ix].vscale_frac_lut0_ntsc);
		wReg32(0x31234, szpar[ix].vscale_frac_lut1_ntsc);
	} else {
		val32 = szpar[ix].vscale_p | (szpar[ix].vscale_r << 4) | (szpar[ix].vscale_d_m1 << 8) | (szpar[ix].vscale_ores_m1 << 10);
		wReg32(0x3122c, val32);
		wReg32(0x31230, szpar[ix].vscale_frac_lut0);
		wReg32(0x31234, szpar[ix].vscale_frac_lut1);
	}
	return;
}

static void atv_be_set_mode(uint32_t mode) 
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	
	switch (mode) {
	case nNoiseReduce:
		if (ISNM600C0(pp->chipid)) {
			val32 = rReg32(0x31204);
			val32 |= (1 << 7);
			wReg32(0x31204, val32);
		
			val32 = rReg32(0x31288);
			val32 &= ~(0x3ff | (0x3ff << 10));
			val32 |= 0x2;
			wReg32(0x31288,  val32);
		
			val32 = rReg32(0x31244);
			val32 &= ~(0x3fff << 16);
			val32 |= (0x200 << 16);
			wReg32(0x31244, val32);
		
			val32 = rReg32(0x31264);
			val32 &= ~(0x7ff);
			val32 |= 0x20;
			wReg32(0x31264, val32);
			
			val32 = rReg32(0x31248);
			val32 &= ~(0x3fff << 16);
			val32 |= (0x17c << 16);
			wReg32(0x31248, val32);
			
			val32 = rReg32(0x31268);
			val32 &= ~(0x7ff);
			val32 |= 0x20;
			wReg32(0x31268, val32);
			
			val32 = rReg32(0x3124c);
			val32 &= ~(0x3fff << 16);
			val32 |= (0x11a << 16);
			wReg32(0x3124c, val32);
			
			val32 = rReg32(0x3126c);
			val32 &= ~0x7ff;
			val32 |= 0x33;
			wReg32(0x3126c, val32);
			
			val32 = rReg32(0x31250);
			val32 &= ~(0x3fff << 16);
			val32 |= (0xb6 << 16);
			wReg32(0x31250, val32);
			
			val32 = rReg32(0x31270);
			val32 &= ~0x7ff;
			val32 |= 0x41;
			wReg32(0x31270, val32);
			
			val32 = rReg32(0x31254);
			val32 &= ~(0x3fff << 16);
			val32 |= (0x66 << 16);
			wReg32(0x31254, val32);
			
			val32 = rReg32(0x31274);
			val32 &= ~(0x7ff);
			val32 |= 0x81;
			wReg32(0x31274, val32);
			
			val32 = rReg32(0x31258);
			val32 &= ~(0x3fff << 16);
			val32 |= (0x40 << 16);
			wReg32(0x31258, val32);
			
			val32 = rReg32(0x31278);
			val32 &= ~(0x7ff);
			val32 |= 0xb6;
			wReg32(0x31278, val32);
			
			val32 = rReg32(0x3125c);
			val32 &= ~(0x3fff << 16);
			val32 |= (0x28 << 16);
			wReg32(0x3125c, val32);
			
			wReg32(0x3127c, 0xb6);
			
			wReg32(0x31260, 0x3fff0000);
			
			wReg32(0x31280, 0x0);
		}
		break;
	case nMotionEnhance:
		if (ISNM600C0(pp->chipid)) {
			wReg32(0x31244, 0x4000021);
			wReg32(0x31248, 0x3910041);
			wReg32(0x3124c, 0x2860082);
			wReg32(0x31250, 0x1440148);
			wReg32(0x31254, 0x81040c);
			wReg32(0x31258, 0x660ccd);
			wReg32(0x3125c, 0x41287a);
			wReg32(0x31260, 0x410000);
			wReg32(0x31264, 0x20);
			wReg32(0x31268, 0x20);
			wReg32(0x3126c, 0x33);
			wReg32(0x31270, 0x41);
			wReg32(0x31274, 0x81);
			wReg32(0x31278, 0xb6);
			wReg32(0x3127c, 0xb6);
			wReg32(0x31280, 0xb6);
			val32 = rReg32(0x31284);
			val32 |= 0x200;
			wReg32(0x31284, val32);

			val32 = rReg32(0x31204);
			val32 |= 0x80;
			wReg32(0x31204, val32);

			val32 = rReg32(0x31288);
			val32 &= ~(0xfffff);
			val32 |= 0x2;
			wReg32(0x31288, val32);

		}
		break;
	default:  
		val32 = rReg32(0x31204);
		val32 |= (0x1 << 7);
		wReg32(0x31204, val32);

		wReg32(0x31288, 0xe1000002);

		wReg32(0x31244, 0x2000021);

		wReg32(0x31248, 0x17c0041);

		wReg32(0x3124c, 0x11a0082);

		wReg32(0x3126c, 0x33);

		wReg32(0x31250, 0xb60148);

		wReg32(0x31270, 0x41);

		wReg32(0x31254, 0x66040c);

		wReg32(0x31274, 0x81);

		wReg32(0x31258, 0x400ccd);

		wReg32(0x31278, 0xb6);

		wReg32(0x3127c, 0xb6);

		wReg32(0x31260, 0x3fff0000);

		wReg32(0x31280, 0);
		break;
	}

	return;

}

static void atv_be_force_color(int en)
{
	uint32_t val32;

	val32 = rReg32(0x31298);
	if (en) {
		val32 &= ~(1 << 21);
	} else {
		val32 |= (1 << 21);
	}
	wReg32(0x31298, val32);

}

static void atv_be_set_field_map(uint32_t fieldmap, tNmiTvStandard std)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	uint32_t out = fieldmap & 0xfff;
	uint32_t shift = (fieldmap >> 16) & 0xfff;
	uint32_t cmb;		

	if (ISNM601B(pp->chipid)) {
		cmb = (out >> shift);
		cmb |= (out << (12 - shift));
		cmb &= 0xfff;
	}

	if (!((std == nNTSC) ||
		(std == nPAL_M) ||
		(std == nPAL_N))) {
		uint32_t bit, i;
		for (i = 0; i < 6; i++) {
			bit = (out >> (2 * i)) & 0x3;
			bit = ((bit >> 1) | (bit << 1)) & 0x3;
			out &= ~(0x3 << (2 * i));
			out |= (bit << (2 * i));
			if (ISNM601B(pp->chipid)) {
				bit = (cmb >> (2 * i)) & 0x3;
				bit = ((bit >> 1) | (bit << 1)) & 0x3;
				cmb &= ~(0x3 << (2 * i));
				cmb |= (bit << (2 * i));
			} 
		}
	}

	val32 = rReg32(0x31200);
	if (ISNM601B(pp->chipid)) {
		val32 &= ~((0xfff << 16)|0xfff);
		val32 |= ((cmb << 16)|(cmb|out));
		wReg32(0x31200, val32);
	} else {
		val32 &= ~0xfff;
		val32 |= (out & 0xfff);
	}
	wReg32(0x31200, val32);

	val32 = rReg32(0x31290);
	val32 &= ~0xfff;
	val32 |= (out & 0xfff);
	wReg32(0x31290, val32);
}

static uint32_t gamma_coeff[51][4]= {
	 {256,          0,          0,          0},
	 {193,        246,       1697,        168},
	 {145,        414,       1469,        276},
	 {109,        524,       1332,        339},
	 { 82,        591,       1261,        370},
	 { 62,        627,       1237,        379},
	 { 46,        639,       1248,        371},
	 { 34,        636,       1282,        352},
	 { 25,        621,       1331,        327},
	 { 19,        598,       1391,        296},
	 { 14,        570,       1457,        264},
	 { 10,        538,       1525,        231},
	 {  7,        505,       1594,        198},
	 {  5,        471,       1662,        166},
	 {  4,        437,       1728,        136},
	 {  2,        403,       1791,        108},
	 {  2,        371,       1850,         81},
	 {  1,        340,       1905,         58},
	 {  1,        310,       1957,         36},
	 {  0,        282,       2004,         17},
	 {  0,        256,          0,          0},
	 {  0,        231,         40,       2033},
	 {  0,        208,         76,       2021},
	 {  0,        186,        108,       2010},
	 {  0,        166,        137,       2002},
	 {  0,        147,        162,       1995},
	 {  0,        130,        184,       1990},
	 {  0,        114,        204,       1986},
	 {  0,         99,        220,       1984},
	 {  0,         86,        234,       1984},
	 {  0,         74,        246,       1985},
	 {  0,         62,        255,       1987},
	 {  0,         52,        262,       1990},
	 {  0,         43,        267,       1994},
	 {  0,         34,        270,       2000},
	 {  0,         27,        271,       2006},
	 {  0,         20,        271,       2013},
	 {  0,         14,        269,       2021},
	 {  0,          9,        266,       2029},
	 {  0,          4,        262,       2038},
	 {  0,          0,        256,          0},
	 {  0,       2045,        249,         10},
	 {  0,       2042,        241,         21},
	 {  0,       2039,        233,         32},
	 {  0,       2037,        223,         44},
	 {  0,       2036,        213,         56},
	 {  0,       2034,        202,         68},
	 {  0,       2034,        190,         80},
	 {  0,       2033,        177,         93},
	 {  0,       2033,        164,        106},
	 {  0,       2033,        151,        119},
};

static void atv_be_gamma_correction(int ix)
{
	uint32_t c0, c1, c2, c3;
	if ((ix < 0) || (ix > 50))
		return;

	c0 = gamma_coeff[ix][0];
	c1 = gamma_coeff[ix][1];
	c2 = gamma_coeff[ix][2];
	c3 = gamma_coeff[ix][3];

	wReg32(0x31318, (c0 | (c1 << 16)));
	wReg32(0x3131c, (c2 | (c3 << 16)));

}


typedef struct {
	uint32_t nominal;
	uint32_t frac;
	uint32_t m;
	uint32_t mclk;
	uint32_t sclk;
	uint32_t inverse;
} tI2sRate;

static tI2sRate i2s_rate[] = {
	{0x199999, 0x3, 0x5, 0xa, 0x50, 0x141},
	{0x155555, 0x2, 0x6, 0xc, 0x60, 0x186},
	{0x1c71c7, 0x1, 0x9, 0x9, 0x48, 0x124},
	{0x1745d1, 0x5, 0xb, 0xb, 0x58, 0x164},
	{0x13b13b, 0x1, 0xd, 0xd, 0x68, 0x1a4},
	{0x111111, 0x1, 0xf, 0xf, 0x78, 0x1e1},
};

static void atv_adec_init(void)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	val32 = rReg32(0x30e00);
	val32 &= ~((1 << 19) | (1 << 20));
	wReg32(0x30e00, val32); 	

	val32 = rReg32(0x20f54);
	val32 &= ~((0x7 << 16) | (0x7 << 20));
	val32 |= 2 << 16;
	val32 |= 2 << 20;
	if (ISNM601B(pp->chipid)) {
		if (chip.inp.i2s == nI2S_ADAC) {
			val32 &= ~((0x7 << 16) | (0x7 << 20));
		}
	}
	wReg32(0x20f54, val32);	

	if (ISNM600(pp->chipid)) {
		val32 = 0x311;
	} else {
		val32 = 0x11;
	}
	wReg32(0x22100, val32);

	if (chip.inp.i2s == nI2S_ADEC) {		
		if (ISNM600C0(pp->chipid)) { 
			wReg32(0x30ee0, 0x155555);
			wReg32(0x30ee4, 0x60002);
			wReg32(0x30ee8, 0x600c);
			wReg32(0x30eec, 0x9186);

			val32 = rReg32(0x20f54);
			val32 &= ~((0x7 << 12) | (0x7 << 16)); 
			val32 &= ~((0x7 << 20) | (0x7 << 24));
			val32 |= 4 << 12;
			val32 |= 4 << 16;
			val32 |= 4 << 20;
			val32 |= 4 << 24;
			wReg32(0x20f54, val32 );
		}
	} else if (chip.inp.i2s == nI2S_STD) {	

		if (chip.inp.i2s_rate < nI2S_RATE_END) {
			tI2sRate ir = i2s_rate[chip.inp.i2s_rate];
			wReg32(0x30ee0, ir.nominal);
			val32 = ir.frac|(ir.m << 16);
			wReg32(0x30ee4, val32);
			val32 = ir.mclk|(ir.sclk<<8);
			wReg32(0x30ee8, val32);
			wReg32(0x30eec, ir.inverse);
		}
		

		val32 = rReg32(0x20f54);
		val32 &= ~((0x7 << 12) | (0x7 << 16)); 
		val32 &= ~((0x7 << 20) | (0x7 << 24));
		val32 |= 1 << 12;
		if (ISNM601B(pp->chipid))
			val32 |= 3 << 16;
		else
			val32 |= 1 << 16;
		val32 |= 1 << 20;
		val32 |= 1 << 24;
		wReg32(0x20f54, val32 );

		if (ISNM601B(pp->chipid)) {
			if (pp->pkg4x4) {
				val32 = 1;
				val32 |= (1 << 4);
				val32 |= (1 << 8);
				wReg32(0x20f58, val32);
			}
		}

		if (ISNM601(pp->chipid)) {
			val32 = rReg32(0x22100);
			val32 &= ~0x1;
			wReg32(0x22100, val32);
		}	

#ifdef PC_ENV
		val32 = rReg32(0x2ec10);
		val32 |= 1<<5;
		wReg32(0x2ec10, val32);
#endif	
	}

	if (ISNM601B0(pp->chipid)) {
		wReg32(0x30f20, 0x1e000000);
		wReg32(0x30f20, 0xe3 );
		wReg32(0x30f24, 0xb0009);
		wReg32(0x30f28, 0xa0069);
		wReg32(0x30ef8, 0x800000);
	}

	wReg32(0x30e50, 0xc0a);

	val32 = (0x5000 | (0x5fff << 15) | (1 << 30));
	wReg32(0x30e24, val32); 

	if (ISNM600(pp->chipid))
		wReg32(0x30e58, 0x1);

	wReg32(0x22108, 0x45600);
	wReg32(0x2210c, 0x49600);

	if (ISNM600(pp->chipid))
		wReg32(0x20f10, 0x01100110);	
	else if (ISNM601A(pp->chipid))
		wReg32(0x20f10, 0x100);	
	else {
		wReg32(0x20f10, 0x2bb22110);	
		wReg32(0x20f10, 0xabb22110);	
	}

	if (ISNM600(pp->chipid)) {
		val32 = rReg32(0x20f90);
		val32 &= ~((1 << 9) | (1 << 10));
		val32 |= (1 << 11);
		wReg32(0x20f90, val32);
	}

	if (ISNM600(pp->chipid))
		pp->vol = 0x28;
	else if (ISNM601A(pp->chipid)) {
		pp->volfm = 0x20;
		pp->volntsc = 0x20;
		pp->volpal = 0x50;
	} else {
		pp->volfm = 0x30;
		pp->volntsc = 0x4c;
		pp->volpal = 0x50;
	}
	
	pp->audfilter = 0x66666666;

	return;
}

static void atv_adec_set_filter(tNmiAudFilter *filt)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	pp->audfilter = ((filt->filter7 & 0x7) << 28) |
								((filt->filter6 & 0x7) << 24) |
								((filt->filter5 & 0x7) << 20) |					
								((filt->filter4 & 0x7) << 16) |
								((filt->filter3 & 0x7) << 12) |
								((filt->filter2 & 0x7) << 8) |
								((filt->filter1 & 0x7) << 4) |
								((filt->filter0 & 0x7) << 0);
}

static void atv_adec_config(tNmiTvStandard std, double pllshift)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32, upllshift;
	tNmiTvStandard std1 = std;

	if (std == nSECAM_B) {
		std = nPAL_B;
	} else if (std == nSECAM_D) {
		std = nPAL_D;
	} else if (std == nSECAM_G) {
		std = nPAL_G;
	} else if (std == nSECAM_H) {
		std = nPAL_H;
	} else if (std == nSECAM_K) {
		std = nPAL_K;
	} else if (std == nSECAM_K1) {
		std = nPAL_K1;
	} 

	nmi_debug(N_VERB, "adec ddfs (%d)\n", adec_ddfs_freq[std]);
	upllshift =  (uint32_t)((double)(adec_ddfs_freq[std])/(1 + pllshift * 0.01));
	if (ISNM601B(pp->chipid)) {
		upllshift <<= 1;
		if (std == nFM_RADIO)
			upllshift >>= 2;
	}
	upllshift &= 0x7ffff;
	upllshift |= (1 << 19);
	wReg32(0x30e1c, upllshift);

	if (pp->standard == std1)
		return;

	if (std == nFM_RADIO) {	
		if (ISNM600C0(pp->chipid)) { 
			val32 = rReg32(0x31204);
			val32 &= ~(1 << 12);
			val32 |= (1 << 14);
			wReg32(0x31204, val32);

			val32 = rReg32(0x22100);
			val32 &= ~(1 << 3);		
			wReg32(0x22100, val32);	

			val32 = rReg32(0x30e64);
			val32 &= ~(0xff << 12);
			val32 |= 0xff << 12;
			val32 &= ~(0xf << 8);
			val32 |= 0xc << 8;
			wReg32(0x30e64, val32);
			
			wReg32(0x30e98, 0xe6543210);

			wReg32(0x30eb4, 0x4477);

			wReg32(0x30e9c, 0x2aaa00);

			wReg32(0x30e68, 0x7060504);

			wReg32(0x30e6c, 0x1b0d08);

			wReg32(0x30e80, 0x6050403);

			wReg32(0x30e84, 0x120a07);

			wReg32(0x30eb8, 0x66666666);

			wReg32(0x30ec8, 0x19bfc);

			wReg32(0x30ecc, 0x134fd6);

			val32 = rReg32(0x22104); 
			val32 &= ~(0x7f << 14);
			val32 |= (0x40 << 14);
			wReg32(0x22104, val32);

			
			wReg32(0x30ed0, 0xa);

		} else {	

			if (ISNM601A(pp->chipid)) {
				val32 = rReg32(0x30f00);
				val32 &= ~0x3;
				val32 |= 0x1;
				wReg32(0x30f00, val32);
			} 
			
			val32 = rReg32(0x30efc);
			val32 &= ~(0x3 << 3);
			val32 &= ~(0xff << 18);
			wReg32(0x30efc, val32);

			
			val32 = rReg32(0x30300);
			val32 &= ~0x1;
			wReg32(0x30300, val32);

			
			if (ISNM600C0(pp->chipid)) 
				wReg32(0x30ed0, 0xc);
			else if (ISNM601A(pp->chipid))
				wReg32(0x30ed0, 0x8);
			else
				wReg32(0x30ed0, 0x4);
		} 

		
		wReg32(0x30e38, 0x0); 

		
		val32 = rReg32(0x30e4c);
		val32 &= ~(0xff << 20);
		if (ISNM600(pp->chipid))
			val32 |= (pp->vol << 20);
		else
			val32 |= (pp->volfm << 20);
		wReg32(0x30e4c, val32);

	} else {		

		if (ISNM600C0(pp->chipid)) {
			val32 = rReg32(0x31204);
			val32 |= (1 << 12);
			val32 &= ~(1 << 14);
			wReg32(0x31204, val32);

			val32 = rReg32(0x22100);
			val32 |= (1 << 3);	
			wReg32(0x22100, val32);	 

			
			
			val32 = rReg32(0x30e64);
			val32 &= ~(0xff << 12);
			val32 |= 0xff << 12;
			val32 &= ~(0xf << 8);
			val32 |= 0xc << 8;
			val32 |= (1 << 31);
			wReg32(0x30e64, val32 );
				
			
			wReg32(0x30e98, 0x76543210);

			
			wReg32(0x30eb4, 0x4477);

			
			wReg32(0x30e9c, 0x2aaa00);

			
			wReg32(0x30e68, 0x7060504);

			
			wReg32(0x30e6c, 0x1b0d08);

			
			wReg32(0x30e80, 0x6050403);

			
			wReg32(0x30e84, 0x120a07 );

			
			wReg32(0x30eb8, pp->audfilter);

			
			wReg32(0x30ec8, 0x19bfc);

			
			wReg32(0x30ecc, 0x1cf7c5);

			
			if (std == nNTSC) {
				
				val32 = rReg32(0x22104); 
				val32 &= ~(0x7f << 14);
				val32 |= (0x7f << 14);
				wReg32(0x22104, val32);
			} else {
				
				val32 = rReg32(0x22104); 
				val32 &= ~(0x7f << 14);
				val32 |= (0x60 << 14);
				wReg32(0x22104, val32);
				wReg32(0x30ed0, 0xa);
			}

			wReg32(0x30ed0, 0xa);

		} else {	
			val32 = rReg32(0x30f00);
			val32 &= ~0x3;
			if (std == nNTSC) 
				val32 |= 0x2;
			wReg32(0x30f00, val32);			

			val32 = rReg32(0x30efc);
			val32 |= (0x3 << 3);
			if (std == nNTSC)
				val32 |= (0xa0 << 18);
			else
				val32 |= (0x64 << 18);
			wReg32(0x30efc, val32);

			
			if (ISNM600C0(pp->chipid)) {
				if (std == nNTSC)
					wReg32(0x30ed0, 0xc);
				else
					wReg32(0x30ed0, 0x8);
			} else if (ISNM601A(pp->chipid))
				wReg32(0x30ed0, 0x8);
			else
				wReg32(0x30ed0, 0xc);

			
			wReg32(0x30300, (0x8 | (atv_standard_params[std].arej)));  
		} 

		
		val32 = rReg32(0x30e4c);
		val32 &= ~(0xff << 20);
		if (ISNM600(pp->chipid)) {
			val32 |= (pp->vol << 20);
			val32 |= (0x7 << 28);
		} else {
			if (std == nNTSC)
				val32 |= (pp->volntsc << 20);
			else
				val32 |= (pp->volpal << 20);
		}
		wReg32(0x30e4c, val32);

		
		wReg32(0x30e38, atv_standard_params[std].emphasis_filter); 
	}

	
	val32 = rReg32(0x30e00);
	if (std == nFM_RADIO) {	
		val32 |= (1 << 21); 
		val32 &= ~((1 << 25) | (1 << 0)); 
	} else {
		val32 |= (1 << 25); 
		val32 &= ~((1 << 21) | (1 << 0)); 
	}
	wReg32(0x30e00, val32);

	if (ISNM601(pp->chipid)) {
		wReg32(0x30ef8,0x1e00000);
	}

	if (ISNM601B(pp->chipid)) {
		val32 = rReg32(0x30f10);
		val32 &= ~0x3;
		if (std == nNTSC)
			val32 |= 0x2;
		else
			val32 |= 0x1;
		wReg32(0x30f10, val32);			
	}

	return;
}

static double atv_adec_agc_gain(void)
{
	uint32_t val32; 
	double	agc;

	val32 = rReg32(0x30e18);
	val32 &= 0x3fffffff;
	agc = (double)val32/(double)(1 << 20);
	return agc; 
}

static int atv_get_rssi(void)
{
	uint32_t val32; 
	double	agc, rssi;
	double agcFe;
	tAtvRFGain rfg;

	val32 = rReg32(0x30e18);
	val32 &= 0x3fffffff;
	agc = (double)val32/(1 << 10);

	rssi = 20 * log10(agc);
	agcFe = atv_fe_agc_gain();
	rssi += 20 * log10(agcFe/4096.);

	atv_rf_agc_gain(&rfg);
	rssi += rfg.lnadb;
	rssi += rfg.bblidb;
	rssi += 6;					
	
	return (int)-rssi; 
}

static int atv_get_overthcount(void)
{
	uint32_t val32;
	val32= rReg32(0x30ef8);
	val32 &=0xffff;
	return val32;
}
static int atv_adec_get_rssi(void)
{
	uint32_t val32; 
	double	agc, rssi;
	double agcFe;
	tAtvRFGain rfg;

	val32 = rReg32(0x2eed4);
	nmi_debug(N_VERB, "0x2eed4 (%08x)\n", val32);
	val32 = rReg32(0x30e18);
	nmi_debug(N_VERB, "0x30e18 (%08x)\n", val32);
	val32 &= 0x3fffffff;
	agc = (double)val32/(1 << 10);

	rssi = 20 * log10(agc);
	agcFe = atv_fe_agc_gain();
	rssi += 20 * log10(agcFe/4096.);

	atv_rf_agc_gain(&rfg);
	rssi += rfg.lnadb;
	rssi += rfg.bblidb;
	rssi += 6;

	rssi = rssi * -1 + 108;
	if (rssi < 0.)
		rssi = 0.;
	if (rssi > 127)
		rssi = 127;
		
	return (int)rssi; 
}

static int atv_adec_agc_lock(void)
{
	uint32_t val32;
	int lock;

	val32 = rReg32(0x30e18);
	if ((val32 >> 30) & 0x1)
		lock = 1;
	else 
		lock = 0;
	return lock;
}

static int atv_adec_lock(void)
{
	uint32_t val32;
	int lock = 0;

	val32 = rReg32(0x30e54);
	if ((val32 >> 31) & 0x1)
		lock = 1;
	else 
		lock = 0;
		 
	return lock;

}

static void atv_adec_get_status(tAtvAdecAqmStatus *p)
{
	uint32_t val1, val2;	
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	if (!ISNM601B(pp->chipid))
		val1 = rReg32(0x30eb0);
	else
		val1 = rReg32(0x30f2c);

	p->stereo = (int)(val1 & 0x1);
	p->filter = (int)((val1 >> 4) & 0x7);
	p->state = (int)((val1 >> 8) & 0x7);

	val1 = rReg32(0x30ea0);
	val1 &= 0xffffff;
	if (val1 >= (1<<19) ) 
		p->levellow = val1 - (1<<20);
	else 
		p->levellow = val1;

	if (!ISNM601B(pp->chipid)) {
		val1 = rReg32(0x30ea8);
		val1 &= 0xffffff;
		if (val1 >= (1<<19) ) 
			p->levelhigh = val1 - (1 << 20);
		else
			p->levelhigh = val1;
	} else {
		val1 = rReg32(0x30f18);	
		if (val1 >= (1<<28)) 
			p->levelhigh = val1 - (1<<29);
		else
			p->levelhigh = val1;
		p->levelhigh >>=16;
		val1 = rReg32(0x30f14);
		p->levellow = val1 >>14;
	}

	val2 = rReg32(0x30ea4);
	val2 &= 0xffffff;
	if ((val1 == 0) || (val2 == 0)) {
		p->snr = 1;
	} else {
		p->snr = (double)val1/(double)val2;
	} 

	val1 = rReg32(0x30e54);
	val1 &= 0x3fffff;
	if (val1 >= (1<<21))
		p->plllevel = val1 - (1<<22);
	else
		p->plllevel = val1;

	p->plllevel /= 1024;

	return;		
}


static void atv_adec_mute(int mute)
{
	uint32_t val32;

	val32 = rReg32(0x22100);
	if (mute)
		val32 |= (1 << 2);
	else
		val32 &= ~(1 << 2);

	wReg32(0x22100, val32);
	return;

}
#if 0

#else
static void atv_adec_adj_volume(int step)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	uint8_t vol;

	
	if (ISNM600(pp->chipid)) {	
		#if 0
		if (step == -1)
			vol = 0x14;
		else if (step == 1)
			vol = 0x56;
		else
			vol = 0x28;
		#else
		    vol = (step*0x56)/7;
		#endif
		pp->vol = vol;		
	} else if (ISNM601A(pp->chipid)) {
		if (pp->standard == nNTSC){
			#if 0
			if (step == -1)
				vol = 0x10;
			else if (step == 1)
				vol = 0x40;
			else
				vol = 0x20;
			#else
		        vol = (step*0x40)/7;
		    #endif
			pp->volntsc = vol;
		} else if (pp->standard == nFM_RADIO){
		    #if 0
			if (step == -1)
				vol = 0x10;
			else if (step == 1)
				vol = 0x40;
			else
				vol = 0x20;
			#else
		        vol = (step*0x40)/7;
		    #endif
			pp->volfm = vol;
		}	else	{
		    #if 0
			if (step == -1)
				vol = 0x28;
			else if (step == 1)
				vol = 0xa0;
			else
				vol = 0x50;
			#else
		    vol = (step*0xa0)/7;
		    #endif
			pp->volpal = vol;
		}
	} else {
		if (pp->standard == nNTSC){		
			#if 0
			if (step == -1)
				vol = 0x26;
			else if (step == 1)
				vol = 0x98;
			else
				vol = 0x4c;
			#else
		        vol = (step*0x98)/7;
		    #endif
			pp->volntsc = vol;
		} else if (pp->standard == nFM_RADIO){
		    #if 0
			if (step == -1)
				vol = 0x18;
			else if (step == 1)
				vol = 0x60;
			else
				vol = 0x30;
			#else
		        vol = (step*0x60)/7;
		    #endif
			pp->volfm = vol;
		}	else	{
		    #if 0
			if (step == -1)
				vol = 0x28;
			else if (step == 1)
				vol = 0xa0;
			else
				vol = 0x50;
			#else
		        vol = (step*0xa0)/7;
		    #endif
			pp->volpal = vol;
		}
	}

	val32 = rReg32(0x30e4c);
	val32 &= ~(0xff << 20);
	val32 |= (vol << 20);
	wReg32(0x30e4c, val32);
	return;
}

#endif
static void atv_demod_reset_start(tNmiTvStandard std)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	if (ISNM600C0(pp->chipid)) {
		if (std != nFM_RADIO) {
			
			wReg32(0x2ec04, 0x6);
			nmi_delay(55);

			
			wReg32(0x2ec04, 0xe);
			nmi_delay(40);

			
			val32 = rReg32(0x22100);
			wReg32(0x22100, (val32 &  ~0x1));			

			
			wReg32(0x2ec04, 0xf);			
		}	

		
		val32 = rReg32(0x22900);
		wReg32(0x22900, (val32 & ~0x1));
	} else {	
#if defined(SYNCMODE)
		atv_vdec_set_hsync_mode(2);
		pp->bloAvg = 0;
#endif
		val32 = rReg32(0x2fe00);
		val32 |= (1 << 15);			
		wReg32(0x2fe00, val32);
		wReg32(0x2ec04, 0xf);				
	}

	return;
}

static void atv_demod_reset(void)
{
	uint32_t i2s;
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	if (ISNM600C0(pp->chipid)) {
		
		if (pp->standard == nFM_RADIO) {
			wReg32(0x2ec04, 0x5);
		} else {
			wReg32(0x2ec04, 0xf);
		}

		
		if (pp->standard == nFM_RADIO) {
			i2s = rReg32(0x22100);
			wReg32(0x22100, (i2s & ~0x1));
		}

		
		if (pp->standard == nFM_RADIO) {
			nmi_delay(5);
		} else {
			nmi_delay(20); 
		}

		
		
		if (pp->standard != nFM_RADIO) {

			val32 = rReg32(0x22900);
	 		wReg32(0x22900, (val32 | 0x1));

			wReg32(0x2ec04, 0x7);
			wReg32(0x2ec04, 0x5);
			wReg32(0x2ec04, 0x1);
			wReg32(0x2ec04, 0x0);
			wReg32(0x20f08, 0x3fffff);
		} else {

			wReg32(0x2ec04, 0xa);

			if (chip.inp.i2s == nI2S_ADEC) {			
				wReg32(0x20f08, 0x3ccdb6);
			} else if (chip.inp.i2s == nI2S_STD) {
				wReg32(0x20f08, 0x3ccdc6);
			}
		} 

		
		i2s = rReg32(0x22100);
		wReg32(0x22100, (i2s | 0x1));

    	
		if (pp->standard != nFM_RADIO) {
	    	wReg32(0x2ef34,0);
	    	nmi_delay(20);
	   		wReg32(0x2ef34, 0x181a);
		}

	} else {	
			
		
		val32 = rReg32(0x2fe00);
		val32 &= ~(1 << 15);		
		wReg32(0x2fe00, val32);
		wReg32(0x2ec04, 0x0);
			
		if (pp->standard == nFM_RADIO) {
			
			val32 = rReg32(0x20f08);
			if (ISNM601A(pp->chipid)) {
				val32 &= ~(0x3f << 22);
				val32 |= (1 << 17);				
			} else {
				val32 &= ~(0x3f << 21);
			}
			wReg32(0x20f08, val32);
		} else {
			
			wReg32(0x20f08, 0xffffffff);
		}

    	
		if (pp->standard != nFM_RADIO) {
	    	wReg32(0x2ef34,0);
	    	nmi_delay(20);
	   		wReg32(0x2ef34, 0x1810);
		}

	}
	return;
}

static int atv_demod_check_lock(uint32_t wait)
{
	uint32_t stim;
	int lock = 0;

	stim = nmi_get_tick();
	do {
		nmi_delay(100);
		
		lock = atv_fe_lock();
		if (lock)
			break;
	} while ((nmi_get_tick() - stim) <= (wait + 100));		

	return lock;
}

static double atv_demod_freq_offset(void)
{
	int val32;
	double foff;
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	val32 = (int)rReg32(0x2ef40);
	if (val32 >= (1 << 29))
		val32 -= (1 << 30);
	foff = (7.858/10000) * val32;
	if (ISNM601B(pp->chipid))
		foff /=2;

	return foff;
}

static int atv_demod_get_pll_level(void)
{
	uint32_t val32;
	int plllevel;

	val32 = rReg32(0x2ef44);
	val32 >>=  10;
	val32 &= 0x3ff;
	
	if (val32 >= (1<<9))
		plllevel = val32 - (1<<10);
	else
		plllevel = val32;

	return plllevel;
}


static double atv_demod_time_offset(void)
{
	double toff;
	int decre, nominal;

	decre = (int)rReg32(0x2fe1c);
	decre &= 0x3fffff;
	if (decre >= (1 << 21))
		decre -= (1 << 22);
	nominal = rReg32(0x2fe10);
	nominal &= 0xffff;

	toff = (double)decre/nominal * 3.8147;

	return toff;

}

static void atv_demod_fb_get_status(tAtvFBStatus *p)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	if (ISNM600(pp->chipid)) {
		p->of = 0;
		p->cf = 0;
		p->util = 0;
	} else {
		val32 = rReg32(0x31368);
		p->of = (val32 >> 24) & 0x1;
		p->cf = (val32 >> 16) & 0x3f;
		p->util = (val32 >> 8) & 0x7f;
	}
}


static void atv_set_scan_snr_level(double snr)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	
	if ((snr < -6.) || (snr > 6.))
		return;

	pp->snrth = snr;

}

static void atv_set_scan_level(int level)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	if (level == 1) {
		pp->alocklevelth = 65;
		pp->linewidthdevth = 2;
	} else if (level == 2) {
		pp->alocklevelth = 60;
		pp->linewidthdevth = 5;
	} else if (level == 3) {
		pp->alocklevelth = 55;
		pp->linewidthdevth = 5;
	} else if (level == 4) {
		pp->alocklevelth = 50;
		pp->linewidthdevth = 5;
	} else if (level == 5) {
		pp->alocklevelth = 45;
		pp->linewidthdevth = 10;
	} else if (level == 6) {
		pp->alocklevelth = 40;
		pp->linewidthdevth = 15;
	} else if (level == 7) {
		pp->alocklevelth = 35;
		pp->linewidthdevth = 1023;
	} 
	
	return;
}

static void atv_set_fast_scan(int en)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	if (en) {
		pp->fastscan = 1;
	} else {
		pp->fastscan = 0;
	}
}

static int atv_scan(tAtvTune *tune)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	int lock = 0, rssi;
	uint32_t val32;
	uint32_t linewidthdev;
	int alocklevel, locklevel;
	int fast_tv_scan=pp->fastscan;

	pp->doscan = 1;

	
	val32 = rReg32(0x31298);
	val32 |= (1 << 21);
	wReg32(0x31298, val32);

	atv_tuner_config(tune);
	nmi_delay(100);

	linewidthdev = atv_vdec_get_avg_line_width_dev();
	val32 = rReg32(0x2ef44);
	locklevel = val32 & 0x3ff;
	locklevel = (locklevel >= 512) ? (locklevel - 1024) : locklevel;
	alocklevel = (val32 >> 10) & 0x3ff;
	alocklevel = (alocklevel >= 512) ? (alocklevel - 1024) : alocklevel;
	nmi_debug(N_SCAN, "1. alocklevel (%d), linewidth deviation (%d)\n", alocklevel, linewidthdev);				

	rssi = atv_fe_get_rssi();

	if (fast_tv_scan == 0) {
		if (alocklevel >= 16) {	

			
			if (rssi > -75) {		
				val32 = rReg32(0x31298);
				val32 &= ~(1 << 21);
				wReg32(0x31298, val32);
			}

			nmi_delay(750);
					
			
			linewidthdev = atv_vdec_get_avg_line_width_dev();
			val32 = rReg32(0x2ef44);
			locklevel = val32 & 0x3ff;
			locklevel = (locklevel >= 512) ? (locklevel - 1024) : locklevel;
			alocklevel = (val32 >> 10) & 0x3ff;
			alocklevel = (alocklevel >= 512) ? (alocklevel - 1024) : alocklevel;
			nmi_debug(N_SCAN, "2. alocklevel (%d), line width deviation (%d)\n", alocklevel, linewidthdev);				
		}

		if ((alocklevel >= pp->alocklevelth) && ((int)linewidthdev <= pp->linewidthdevth)) {
			double snr;

			lock = 1;
			snr = atv_vdec_get_noise_var();
			if (snr < pp->snrth)
				lock = 0;				
		} else {
			lock = 0;
		}
	} else {
		if (alocklevel >= pp->alocklevelth) {
			lock = 1;
		} else {
			lock=0;
		}			
	}

	pp->doscan = 0;

	return lock;
}

static void atv_bt656_set_size(tNmiOutputSize ix)
{
	tNmiPriv 	*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;

	if (ix > n480x360)
		return;

	
	chip.inp.size = ix;

	
	atv_demod_reset_start(pp->standard);

	
	
	if (ISNM601(pp->chipid)) {
		val32 = rReg32(0x31218);
		if (szpar[ix].width == 220) {
			val32 &= ~(0xf << 20);
			val32 |= (4 << 20);
		} else {
			val32 &= ~(0xf << 20);
		}
		wReg32(0x31218, val32);

		if ((szpar[ix].width >= 320) && (szpar[ix].height >= 240)) {
			wReg32(0x30238, 0x31);
		} else if ((szpar[ix].width >= 220) && (szpar[ix].height >= 176)) {
			wReg32(0x30238, 0x21);
		} else if ((szpar[ix].width >= 176) && (szpar[ix].height >= 144)) {
			wReg32(0x30238, 0x11);
		} else {
			wReg32(0x30238, 0x01);
		}
	}
	val32 = szpar[ix].hscale_p | 
				(szpar[ix].hscale_r << 4) | 
				((szpar[ix].hscale_d_m1 & 0x3) << 8) | 
				(szpar[ix].hscale_ores_m1 << 10) | 
				(0x3 << 20) |
				(((szpar[ix].hscale_d_m1 >> 2) & 0x1) << 22);
	wReg32(0x31220, val32);
	wReg32(0x31224, szpar[ix].hscale_frac_lut0);
	wReg32(0x31228, szpar[ix].hscale_frac_lut1);

	
	if (ISNM601(pp->chipid)) {
		if (szpar[ix].height == 180) {
			szpar[ix].vscale_p = 2;
			szpar[ix].vscale_r = 1;
			szpar[ix].vscale_d_m1 = 0;
			szpar[ix].vscale_ores_m1 = 191;
			szpar[ix].vscale_frac_lut0 = 0x80;
			szpar[ix].vscale_frac_lut1 = 0;

			szpar[ix].vscale_p_ntsc = 4;
			szpar[ix].vscale_r_ntsc = 1;
			szpar[ix].vscale_d_m1_ntsc = 0;
			szpar[ix].vscale_ores_m1_ntsc = 191;
			szpar[ix].vscale_frac_lut0_ntsc = 0xc840;
			szpar[ix].vscale_frac_lut1_ntsc = 0;
		}
	}
	if (pp->standard == nNTSC) {
		val32 = szpar[ix].vscale_p_ntsc | (szpar[ix].vscale_r_ntsc << 4) | (szpar[ix].vscale_d_m1_ntsc << 8) | (szpar[ix].vscale_ores_m1_ntsc << 10);
		wReg32(0x3122c, val32);
		wReg32(0x31230, szpar[ix].vscale_frac_lut0_ntsc);
		wReg32(0x31234, szpar[ix].vscale_frac_lut1_ntsc);

		val32 = rReg32(0x31204);
		if (szpar[ix].height < 240) {
			val32 |= (1 << 5);
		} else {
			val32 &= ~(1 << 5);
		}

		if (ISNM600(pp->chipid)) {
			uint32_t val1;

			val32 &= ~(1 << 16);

			if ((szpar[ix].width == 426) &&
				(szpar[ix].height == 320)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x4;
				val1 |= (0x3 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (425 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xc840);
				wReg32(0x31308, 0);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x4;
				val1 |= (0x3 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (319 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0xc840);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}

			if ((szpar[ix].width == 480) &&
				(szpar[ix].height == 360)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x3;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (479 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xb50);
				wReg32(0x31308, 0);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x3;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (359 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0xb50);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}

			if ((szpar[ix].width == 480) &&
				(szpar[ix].height == 280)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x3;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (479 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xb50);
				wReg32(0x31308, 0);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x7;
				val1 |= (0x6 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (279 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0xeb97520);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}

			if ((szpar[ix].width == 360) &&
				(szpar[ix].height == 240)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x9;
				val1 |= (0x8 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (359 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xcb975420);
				wReg32(0x31308, 0xe);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x2;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (239 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0x0);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}
		}
		wReg32(0x31204, val32);

	} else {
		val32 = szpar[ix].vscale_p | (szpar[ix].vscale_r << 4) | (szpar[ix].vscale_d_m1 << 8) | (szpar[ix].vscale_ores_m1 << 10);
		wReg32(0x3122c, val32);
		wReg32(0x31230, szpar[ix].vscale_frac_lut0);
		wReg32(0x31234, szpar[ix].vscale_frac_lut1);

		val32 = rReg32(0x31204);
		if (pp->standard == nPAL_M) {
			if (szpar[ix].height < 240) {
				val32 |= (1 << 5);
			} else {
				val32 &= ~(1 << 5);
			}
		}

		if (ISNM600(pp->chipid)) {
			uint32_t val1;

			val32 &= ~(1 << 16);

			if ((szpar[ix].width == 426) &&
				(szpar[ix].height == 320)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x4;
				val1 |= (0x3 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (425 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xc840);
				wReg32(0x31308, 0);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x4;
				val1 |= (0x3 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (319 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0xc840);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}

			if ((szpar[ix].width == 480) &&
				(szpar[ix].height == 360)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x3;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (479 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xb50);
				wReg32(0x31308, 0);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x3;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (359 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0xb50);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}

			if ((szpar[ix].width == 480) &&
				(szpar[ix].height == 280)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x3;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (479 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xb50);
				wReg32(0x31308, 0);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x7;
				val1 |= (0x6 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (279 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0xeb97520);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}

			if ((szpar[ix].width == 360) &&
				(szpar[ix].height == 240)) {
				val1 = rReg32(0x31300);
				val1 &= ~(0xff);
				val1 |= 0x9;
				val1 |= (0x8 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (359 << 10);
				wReg32(0x31300, val1);
				wReg32(0x31304, 0xcb975420);
				wReg32(0x31308, 0xe);

				val1 = rReg32(0x3130c);
				val1 &= ~(0xff);
				val1 |= 0x2;
				val1 |= (0x2 << 4);
				val1 &= ~(0x3ff << 10);
				val1 |= (239 << 10);
				wReg32(0x3130c, val1);
				wReg32(0x31310, 0x0);
				wReg32(0x31314, 0x0);

				val32 |= (1 << 16);
			}
		}
		wReg32(0x31204, val32);
	}

	
	val32 = szpar[ix].h_trig_thrd | (szpar[ix].v_trig_thrd << 10) | (1 << 19) | (1 << 20);
	if (chip.inp.input_order) {	
		val32 &= ~(1 << 19);
	}
	if (chip.inp.btformat == nBTRGB) {
		val32 |= (1 << 23);			
		val32 &= ~(1 << 20);		
		val32 &= ~(1 << 19);		
	}
	wReg32(0x31298, val32);
	if (ISNM600(pp->chipid))
		wReg32(0x31290, szpar[ix].fb_cfg);
	else {
		double hr = (double)szpar[ix].width;
		double ratio;
		uint32_t thrd;

		val32 = rReg32(0x31290);
		val32 &= ~(0x1ff << 12);
		switch (chip.inp.btrate) {
		case nClk6_75:
			ratio = 4.0;
			break;
		case nClk13_5:
			ratio = 2.0;
			break;
		case nClk27:
		default:
			ratio = 1.0;
			break;
		}
		if ((704./hr) < ratio)
			thrd = 32;
		else if (((hr - 16.)*2) > 480)
			thrd = 480;
		else 
			thrd = (uint32_t)((hr - 16) * 2);
		val32 |= (thrd << 12);
		wReg32(0x31290, val32);
	} 

	if (ISNM600(pp->chipid)) {
		val32 = rReg32(0x31294);
		val32 |= 1;
		wReg32(0x31294, val32);
	}

	switch (chip.inp.btrate) {
	case nClk6_75:
		if (ISNM600(pp->chipid)) {
			wReg32(0x22928, szpar[ix].f0_last_line_6p75);
			wReg32(0x22930, szpar[ix].f1_last_line_6p75);
			wReg32(0x2293c, szpar[ix].last_line_6p75);
			wReg32(0x22940, (135+szpar[ix].last_pixel_6p75));	

			val32 = rReg32(0x31298);
			val32 &= ~(0x1ff << 10);
			val32 |= (16 << 10);
			wReg32(0x31298, val32);
			wReg32(0x22900, 0x3024409);
		} else {
			val32 = rReg32(0x31294);
			val32 |= (1 << 29);
			wReg32(0x31294, val32);
			val32 = rReg32(0x31290);
			val32 &= ~(0x1f << 27);
			val32 |= (8 << 27);
			wReg32(0x31290, val32);
		}
		break;
	case nClk13_5:
		if (ISNM600(pp->chipid)) {
			wReg32(0x22928, szpar[ix].f0_last_line_13p5);
			wReg32(0x22930, szpar[ix].f1_last_line_13p5);
			wReg32(0x2293c, szpar[ix].last_line_13p5);
			wReg32(0x22940, (chip.inp.blank_interval+szpar[ix].last_pixel_13p5));	
		} else {
			val32 = rReg32(0x31294);
			val32 |= 0x1;
			val32 &= ~(1 << 29);
			wReg32(0x31294, val32);
		}
		break;
	case nClk27:
	default:
		if (ISNM600(pp->chipid)) {
			wReg32(0x22928, szpar[ix].f0_last_line_27);
			wReg32(0x22930, szpar[ix].f1_last_line_27);
			wReg32(0x2293c, szpar[ix].last_line_27);
			wReg32(0x22940, (chip.inp.blank_interval+szpar[ix].last_pixel_27));	
		} else {
			val32 = rReg32(0x31294);
			val32 &= ~0x1;
			val32 &= ~(1 << 29);
			wReg32(0x31294, val32);
		}
		break;
	}


	atv_demod_reset();

	return;
}

static void atv_bt656_config(void)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;
	uint32_t val32;
	int ix = chip.inp.size;

	if (ISNM600(pp->chipid))
		wReg32(0x22900, 0xe);
	

	wReg32(0x20f50, 0x11111111);


	val32 = rReg32(0x20f54);
	val32 &= ~0xfff;
	val32 |= 0x111;
	wReg32(0x20f54, val32);


	if (ISNM601B(pp->chipid)) {
		if (pp->pkg4x4) {
			val32 = (0x2 << 4);	
			val32 |= (0x4 << 8);	
			wReg32(0x20f58, val32);
		} else {
			wReg32(0x20f58, 0x0);
		}
	}

	

	if (ISNM600(pp->chipid)) {
		val32 = rReg32(0x20f90);
		val32 &= ~0x1c0;
		val32 |= 0x800;
		wReg32(0x20f90, val32);
	}

	wReg32(0x22904, 0x31294);
	if (ISNM600(pp->chipid))
		wReg32(0x22908, 0x31294);

	if (chip.inp.clk_polarity ||
			chip.inp.vsync_polarity ||
			chip.inp.hsync_polarity) {
		val32 = rReg32(0x20f60);
		val32 |= (chip.inp.clk_polarity)?(1 << 14):0;
		val32 |= (chip.inp.vsync_polarity)?(1 << 13):0;
	 	val32 |= (chip.inp.hsync_polarity)?(1 << 12):0;
		wReg32(0x20f60, val32);
	}

	if (ISNM601(pp->chipid)) {
		val32 = rReg32(0x31294);
		val32 &= ~(0x7 << 1);
		val32 |= (1 << 25);
		wReg32(0x31294, val32);
	} 

	if (chip.inp.vbl < 2)
		chip.inp.vbl = 2;

	switch (chip.inp.btrate) {
	case nClk6_75:
		if (ISNM600(pp->chipid)) {
			wReg32(0x2ec10, 0x8); 		
			wReg32(0x22934, 3);
			wReg32(0x22924, (chip.inp.vbl - 2) + 3);
			wReg32(0x22938, 3);
			wReg32(0x2292c, (chip.inp.vbl - 2) + 3);
			wReg32(0x22928, (chip.inp.vbl - 2) + szpar[ix].f0_last_line_6p75);
			wReg32(0x22930, (chip.inp.vbl - 2) + szpar[ix].f1_last_line_6p75);
			wReg32(0x2293c, (chip.inp.vbl - 2) + szpar[ix].last_line_6p75);
			wReg32(0x22940, (135+szpar[ix].last_pixel_6p75));	
			wReg32(0x22944, 135);
			wReg32(0x22948, (135+4));
			wReg32(0x2294c, 34);
			wReg32(0x22950, 34);

			val32 = rReg32(0x31298);
			val32 &= ~(0x1ff << 10);
			val32 |= (16 << 10);
			wReg32(0x31298, val32);
			wReg32(0x22900, 0x3024409);

			val32 = rReg32(0x20f60);
			val32 |= (1 << 10);
			val32 &= ~(1 << 9);
			wReg32(0x20f60, val32);
		} else {
			val32 = rReg32(0x31294);
			val32 |= (1 << 29);
			wReg32(0x31294, val32);
			val32 = rReg32(0x31290);
			val32 &= ~(0x1f << 27);
			val32 |= (8 << 27);
			wReg32(0x31290, val32);
		}
		break;
	case nClk13_5:
		if (ISNM600(pp->chipid)) {
			wReg32(0x2ec10, 0x4); 							
			wReg32(0x22934, 3);
			wReg32(0x22924, (chip.inp.vbl - 2) + 3);
			wReg32(0x22938, 3);
			wReg32(0x2292c, (chip.inp.vbl - 2) + 3);
			wReg32(0x22928, (chip.inp.vbl - 2) + szpar[ix].f0_last_line_13p5);
			wReg32(0x22930, (chip.inp.vbl - 2) + szpar[ix].f1_last_line_13p5);
			wReg32(0x2293c, (chip.inp.vbl - 2) + szpar[ix].last_line_13p5);
			wReg32(0x22940, (chip.inp.blank_interval+szpar[ix].last_pixel_13p5));	
			wReg32(0x22944, (chip.inp.blank_interval));
			wReg32(0x22948, (4+chip.inp.blank_interval));
			wReg32(0x2294c, 68);
			wReg32(0x22950, 68);
			wReg32(0x22900, 0x3024409);		


			val32 = rReg32(0x20f60);
			val32 |= (1 << 10) | (1 << 9);
			wReg32(0x20f60, val32);
		} else {
			val32 = rReg32(0x31294);
			val32 |= 0x1;
			val32 &= ~(1 << 29);
			wReg32(0x31294, val32);
		}
		break;
	case nClk27:
	default:
		if (ISNM600(pp->chipid)) {
			wReg32(0x2ec10, 0x0); 
			wReg32(0x22934, 3);
			wReg32(0x22924, (chip.inp.vbl - 2) + 3);
			wReg32(0x22938, 3);
			wReg32(0x2292c, (chip.inp.vbl - 2) + 3);
			wReg32(0x22928, (chip.inp.vbl - 2) + szpar[ix].f0_last_line_27);
			wReg32(0x22930, (chip.inp.vbl - 2) + szpar[ix].f1_last_line_27);
			wReg32(0x2293c, (chip.inp.vbl - 2) + szpar[ix].last_line_27);
			wReg32(0x22940, (chip.inp.blank_interval+szpar[ix].last_pixel_27));	
			wReg32(0x22944, (chip.inp.blank_interval));
			wReg32(0x22948, (4+chip.inp.blank_interval));
			wReg32(0x2294c, 135);
			wReg32(0x22950, 135);
			wReg32(0x22900, 0x3024409);

			val32 = rReg32(0x20f60);
			val32 &= ~(1 << 10);
			wReg32(0x20f60, val32);
		} else {
			val32 = rReg32(0x31294);
			val32 &= ~0x1;
			val32 &= ~(1 << 29);
			wReg32(0x31294, val32);
		}
		break;
	}

	return;

}


static void atv_cc_init()
{
	uint32_t val32;

	val32 = (140 | (323 << 9));
	wReg32(0x2ff14, val32);

	val32 = (323-140)*192;
	wReg32(0x2ff18, val32);

	val32 = (374) | ((18) << 9) | ((256*19) << 14);
	wReg32(0x2ff1c, val32);

	val32 = (1 << 0) | (17 << 2) | (4 << 7) | (8 << 12) | (1 << 10);
	wReg32(0x2Ff10, val32);


	val32 = rReg32(0x302a0);
	val32 |=	0x6;
	wReg32(0x202a0, val32);
}

static uint32_t atv_get_chipid(void) 
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	uint32_t chipid;
	uint32_t val32;

	chipid	= rReg32(0x20f00);
	if (ISNM601A0(chipid)) {
		val32 = rReg32(0x210fc);
		if ((val32 & 0xff) == 0x2)
			chipid = 0x601a1;
	} else if (ISNM601B0(chipid)) {
		if ((chipid >> 28) & 0x1)
			pp->pkg4x4 = 1;
	}
	pp->chipid = chipid;

	return chipid;
}

static void atv_demod_init(void)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;

	atv_fe_init();
	atv_vdec_init();
	atv_be_init();
	atv_adec_init();

#if 0
	if (ISNM600C0(pp->chipid)) {
		atv_cc_init();
	}	
#endif

	atv_bt656_config();


	pp->alocklevelth = 45;
	pp->linewidthdevth = 10;


	pp->fmscanth = 3;

	pp->uvfact = 1.;


	pp->minScale = 0.;


	if (ISNM601(pp->chipid))
		pp->localpeak = 1;

	pp->bloAvg = 0.0;
	pp->i_nv   = 0;
	pp->decreAvg = 0.0;
	pp->decreDiff = 0;
	pp->snrth = -6.;

	return;
}

const static uint8_t rfinit_c0[] = {
 0x9f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x80, 0x0f, 0x08, 0x04, 0x0f, 0xfe, 0x00, 0xc1, 0xf8,
 0x0f, 0x0f, 0x00, 0x00, 0x18, 0x15, 0x00, 0xff, 0x83, 0x82, 0x23, 0xd7, 0x49, 0x80, 0x86, 0xd0,
 0xe0, 0xd8, 0x05, 0x00, 0x00, 0x63, 0xc0, 0xb0, 0x03, 0x44, 0x00, 0x00, 0x00, 0x03, 0x8f, 0x80,
 0x01, 0x44, 0x80, 0x01, 0x00, 0x01, 0x00, 0x81, 0x80, 0x01, 0xee, 0x80, 0x01, 0x00, 0x01, 0x00,
 0x00, 0x00, 0x00
};

const static uint8_t rfinit_601_a0[] = {
 0x9f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x17, 0x00, 0x0f, 0x08, 0x04, 0x0f, 0x06, 0x00, 0xc1, 0xf8,
 0x0f, 0x0f, 0x00, 0x00, 0x18, 0x15, 0x00, 0xaa, 0x83, 0x82, 0x23, 0xd7, 0x49, 0x80, 0x86, 0xd0,
 0xe0, 0xd8, 0x05, 0x00, 0x00, 0x63, 0xc0, 0xb0, 0x03, 0x44, 0x00, 0x00, 0x00, 0x03, 0x8f, 0x80,
 0x01, 0x44, 0x80, 0x01, 0x00, 0x01, 0x00, 0x81, 0x80, 0x01, 0xfa, 0x80, 0x01, 0x00, 0x01, 0x00,
 0x00, 0x00, 0x00
};

#if 1
const static uint8_t rfinit_601_b0[] = {
 0x5f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x80, 0x08, 0x84, 0x0f, 0x06, 0x00, 0xc1, 0x04,
 0x0f, 0x0f, 0x00, 0x00, 0x18, 0x15, 0x00, 0xaa, 0x03, 0x82, 0x23, 0xd7, 0x49, 0x80, 0x86, 0xd0,
 0xe0, 0xd8, 0x05, 0x00, 0x99, 0x09, 0xc0, 0xb0, 0x00, 0x44, 0x00, 0x00, 0x00, 0x03, 0x8f, 0x80,
0x01, 0x44, 0x80, 0x01, 0x00, 0x01, 0x00, 0x81, 0x80, 0x01, 0xfa, 0x80, 0x01, 0x00, 0x01, 0x00,
0x00, 0x00, 0x00
};
#endif


static void atv_tuner_init(void)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;
	const uint8_t *cfg = NULL;
	
	if (ISNM600C0(pp->chipid)) {
		cfg = rfinit_c0;
	} else if (ISNM601A(pp->chipid)) {
		cfg = rfinit_601_a0;
	} else {
		cfg = rfinit_601_b0;
	} 

	if (cfg != NULL) {
		atvRfburstWrite(0x0, &cfg[0], 63);
	}

	if (chip.inp.io == n1_8Volt) {
		atvrfwrite(0x26, 0xc4);
	}

	if (chip.inp.mode == 1) {
		atvrfwrite(0x37, 0xf0);
	} else if (chip.inp.mode == 2) {
		atvrfwrite(0x37, 0xef);
	} else if (chip.inp.mode == 3) {
		atvrfwrite(0x37, 0xee);
	}

	return;
}

static void atv_show_color_bar(int en)
{
	int ix = chip.inp.size;
	uint32_t val32;

	if (en) {
		val32 = rReg32(0x2ec04);
		val32 |= 0x2;
		val32 &= 0xfffffff7;
		wReg32(0x2ec04, val32);

		val32 = rReg32(0x31204);
		val32 &= 0xfffffdfe;
		wReg32(0x31204, val32);

		wReg32(0x312b0, 0x0);

		val32 = rReg32(0x31204);
		val32 |= 0x1;
		wReg32(0x31204, val32);

		val32 = rReg32(0x312b0);
		val32 |= 1;
		val32 |= (4 << 2);
		val32 |= (3 << 6);
		val32 |= (1 << 25);		
		val32 |= ((szpar[ix].width/8)&0x3f) << 26; 
		wReg32(0x312b0, val32);
	} else {
		val32 = rReg32(0x312b0);
		val32 &= ~0x1;
		val32 &= ~(1 << 25);
		wReg32(0x312b0, val32);
	}
}

static int atv_chip_init(void)
{
	uint32_t 		val32;
	tNmiPriv		*pp = (tNmiPriv *)&chip.priv;

	val32 = atv_get_chipid();
	nmi_debug(N_INFO, "Chip ID (%08x)\n", val32);
	
	if (!(ISNM600(val32) || ISNM601(val32))) {
		nmi_debug(N_ERR,"Wrong Chip ID (%08x), Check the Bus\n", val32);
		return -1;
	}
	pp->chipid = val32;

	if (chip.inp.enablepattern) {
		if (ISNM600(pp->chipid)) {
	      atv_update_pll();

			wReg32(0x21048, 0x8);                  

			val32 = rReg32(0x20f60);
	      val32 |= 0x100;
	      wReg32(0x20f60,val32);

	     	wReg32(0x21040,0x3022);
			wReg32(0x21040,0x3042);
	      	nmi_delay(100);

	      	wReg32(0x20f04, 0);
			nmi_delay(5);  
		}  
		pp->pattern = 1;  
	}

	if (ISNM600(pp->chipid) || ISNM601A(pp->chipid)) {
		wReg32(0x20f14, 0x0884ec4f);
		wReg32(0x20f18, 0x170cd353);
		wReg32(0x20f1c, 0x80000009);
	} else {
		wReg32(0x20f14, 0xa6c00000);
		wReg32(0x20f18, 0x8f1e3999);
		wReg32(0x20f1c, 0x0008a500);
		if (chip.inp.i2s == nI2S_ADAC)
			wReg32(0x20f70, 0x16fa6);
		else
			wReg32(0x20f70, 0x1ffa6);
	}
	atv_update_pll();

	if (ISNM600C0(pp->chipid)) {
		wReg32(0x20f08, 0x3fffff);
	} else {
		wReg32(0x20f08, 0xffffffff);
	} 

	if (ISNM600C0(pp->chipid) ||
			ISNM601A(pp->chipid)) {
		val32 = (1 << 6) | (1 << 7);
		wReg32(0x20f64, val32);
	} else {
		wReg32(0x20f6c, 0x60);
	}
	
	wReg32(0x20f0c, 0x0);
	if (ISNM600(pp->chipid))
		wReg32(0x20f20, 0x0);
	else {
		if (chip.inp.i2s == nI2S_SIGDEL) {
			val32 = rReg32(0x20f60);
			val32 &= ~(0x3 << 5);
			if (chip.inp.io == n2_8Volt) {
				if (ISNM601B(pp->chipid))
					val32 |= (1 << 5);
				wReg32(0x20f20, 0xfc0);
			} else {
				val32 |= (1 << 5);
				wReg32(0x20f20, 0xd80);
			}
			wReg32(0x20f60, val32);
		} else
			wReg32(0x20f20, 0x0);
	}

	val32 = rReg32(0x20f68);
	val32 |= 0x37ff;
	wReg32(0x20f68, val32);

	wReg32(0x20730, 0x200);
	wReg32(0x20704, 0x60);
	wReg32(0x20700, 0x0);

	atv_tuner_init();

	atv_demod_init();


	return 1;
}

static void atv_show_pattern(int en)
{
	uint32_t val32;
	tNmiPriv		*pp = (tNmiPriv *)&chip.priv;

	if (ISNM600(pp->chipid))
		if (!pp->pattern)
			return;

	if (en) {
		if (ISNM600(pp->chipid)) {
			wReg32(0x2ec04, 0x7); 

			val32 = rReg32(0x22900);
			val32 &= ~(1 << 25);
			wReg32(0x22900, val32);

			val32 = 255;   
			wReg32(0x22934, val32);
			wReg32(0x22924, val32);
			wReg32(0x22928, (val32+239));
			wReg32(0x22938, val32);
			wReg32(0x2292c, val32);
			wReg32(0x22930, (val32+239));
			wReg32(0x2293c, (val32+239));                  
			val32 = rReg32(0x22900);
			wReg32(0x22900, (val32 & ~0x1));
			wReg32(0x22900, val32);
		} else {
			atv_show_color_bar(1);
		}
	} else { 
		if (ISNM600(pp->chipid)) {
			val32 = rReg32(0x22900);
			val32 |= (1 << 25);
			wReg32(0x22900, val32);

	      val32 = rReg32(0x22900);
	      wReg32(0x22900, (val32 & ~0x1));
	      wReg32(0x2ec04, 0xf);
	      wReg32(0x22900, val32);
		} else {
			atv_show_color_bar(0);
		}
	}

	return;
}

static void atv_sleep(int ok)
{
	if (ok) {
		wReg32(0x21901, 0x0);
	} else {
		wReg32(0x21901, 0x1);
		nmi_delay(250);
	}
	
}

static void atv_fpga_play(void)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;
	if (pp->pkg4x4)
		atv_toggle_gpio(14);
	else
		atv_toggle_gpio(16);
}


static void atv_toggle_gpio(uint32_t pin)
{
	wReg32(0x20100, (1 << pin));
	wReg32(0x20104, (1 << pin));
	nmi_delay(10);
	wReg32(0x20104, 0x0);
	wReg32(0x20100, 0x0);
	wReg32(0x20104, (1 << pin));
}
static void fm_set_scan_level(int level)
{
	tNmiPriv *pp = (tNmiPriv *)&chip.priv;


	pp->fmscanth = level; 

	return;
}

static void fm_get_window_rssi(int *orssi, int len, int *wrssi,int *overth)
{
	int k;
	for(k=0;k<len;k++){
		*(overth+k)=(*(orssi+k)>>8);
		*(orssi+k)=(*(orssi+k))&0xff;
	}
	for (k=0; k<len; k++) {
		if (k==0)
			*(wrssi+k)=*(orssi+k)+*(orssi+k)+*(orssi+k+1);
		else if (k==len-1)
			*(wrssi+k)=*(orssi+k-1)+*(orssi+k)+*(orssi+k);
		else
			*(wrssi+k)=*(orssi+k-1)+*(orssi+k)+*(orssi+k+1);

		*(wrssi+k) = *(wrssi+k)/3;
	}
}

static int fm_find_peak(int *rssi, int len)
{
	int pos,peak,k;

	pos=-1;
	peak=0;
	for (k=0; k<len; k++) {
		if (*(rssi+k) > peak) {
	   		peak=*(rssi+k);
	    	pos=k;
  		}
	}

	return pos;
}

static int fm_find_dip(int *overth, int len)
{
	int pos,dip,k;
	pos=-1;
	dip=  65535;
	for (k=0; k<len; k++) {
		if (*(overth+k) < dip) {
	   		dip=*(overth+k);
	    	pos=k;
  		}
	}
	return pos;
}

static void fm_select_channel_list(int *orssi, int *wrssi, int *clist, int *nc, int cmax, int clen, int *overth)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;
  	int done=0;
  	int num=0;
  	int posN2;
  	int posP2;
	int posN1;
  	int posP1;
  	int k,idx;
  	int pos;
  	int r_cond,l_cond,least_cond;
  	int relative_th = pp->fmscanth;
  	int least_th=10;
	int dev;
	int or_cond,ol_cond;
	int wrelative_th=1;
	int relative_ratio_th=2;
	int max_overth=250;
	int posL,posR;
	int relative_th_side=2;
	int use_new_search=1;

	if (!ISNM601(pp->chipid)) {
		use_new_search=0;
	}

	nmi_debug(N_VERB, "clen(%d)\n", clen);

	if (clen == 206) {
		dev = 2;
	} else {
		dev = 4;
	}
    
 	while (!done) {
		if (use_new_search==0) {
			pos = fm_find_peak(wrssi, clen);
		} else {
			pos = fm_find_dip(overth, clen);
		}
      
      if (pos==-1) {
          done=1;
      } else {
          posN2 = (pos-dev)<0?(pos+dev):(pos-dev);
          posN2 = (posN2>=clen)?(clen-1):posN2;
          posP2 = ((pos+dev)>=clen)?(pos-dev):(pos+dev);
          posP2 = (posP2<0)?0:posP2;
                    
			posN1 = (pos-dev/2)<0?(pos+dev/2):(pos-dev/2);
			posN1 = (posN1>=clen)?(clen-1):posN1;
			posP1 = ((pos+dev/2)>=clen)?(pos-dev/2):(pos+dev/2);
			posP1 = (posP1<0)?0:posP1;
			posR = ((pos+1)>=clen)?(pos-1):(pos+1);
			posR = (posR<0)?0:posR;
			posL = (pos-1)<0?(pos+1):(pos-1);
			posL = (posL>=clen)?(clen-1):posL;
			if (use_new_search == 0) {
				l_cond = ((*(wrssi+pos)) >= (*(orssi+posN2)+wrelative_th));
				r_cond = ((*(wrssi+pos)) >= (*(orssi+posP2)+wrelative_th));
				ol_cond = ((*(orssi+pos)) >= (*(orssi+posN1)+relative_th));
				or_cond = ((*(orssi+pos)) >= (*(orssi+posP1)+relative_th));
				if (clen != 206) {
					if (ol_cond)
						ol_cond = ((*(orssi+posL)) >= (*(orssi+posN1)+relative_th_side));
					if (or_cond)
						or_cond = ((*(orssi+posR)) >= (*(orssi+posP1)+relative_th_side));
				}
				least_cond = (*(orssi+pos) >= least_th);
			} else {
				l_cond = ((*(overth+pos))*relative_ratio_th) <= (*(overth+posN1));
				r_cond = ((*(overth+pos))*relative_ratio_th) <= (*(overth+posP1));

				nmi_debug(N_VERB, "(%d)pos(%d)(%d)(%d):(%d)(%d)(%d)\n", dev, pos, posN1, posP1, *(orssi+pos), *(orssi+posN1), *(orssi+posP1));
				  
				ol_cond = ((*(orssi+pos)) >= (*(orssi+posN1)+relative_th));
				or_cond = ((*(orssi+pos)) >= (*(orssi+posP1)+relative_th));
				least_cond = (*(orssi+pos) >= least_th) && ((*(overth+pos))<=max_overth);
			}

			if (l_cond && r_cond && least_cond && or_cond && ol_cond) {

				*(clist+num)=pos;	
                  
				if (use_new_search==0){    
					for (k=-2;k<3;k++) {
					  idx=pos+k;
					  idx=(idx<0)?0:idx;
					  idx=(idx>=clen)?clen-1:idx;
					  *(wrssi+idx)=-1;
					}
				} else {
					for(k=-1*dev/2;k<=dev/2;k++){
						idx=pos+k;
						idx=(idx<0)?0:idx;
						idx=(idx>=clen)?clen-1:idx;
						*(overth+idx)=65535;
					}
			  	}
             num++;
				if (num>=cmax)
					done=1;
			} else {
				if (use_new_search==0){
				  *(wrssi+pos)=-1;
				} else {
					*(overth+pos)=65535;
			  	}
			}
		}
	}

	*(nc)=num;
}

static int fm_seek_select(int *rssi, int *overth, int sz)
{
	tNmiPriv 		*pp = (tNmiPriv *)&chip.priv;
	int i;
	int aRssi[3];
	int mRssi = 0, pos = 0;
	int start_cond, end_cond, least_cond;
	int relative_th=pp->fmscanth;
	int wrelative_th=1;
	int least_th=10;
	int found = 0;
	int start_ocond=1, end_ocond=1;
	int relative_ratio_th=2;
	int relative_th_side=2;
	int use_new_search=1;

	if (!ISNM601(pp->chipid)) {
		use_new_search=0;
	}

	if (sz == 7) {
		aRssi[0] = (rssi[0] + rssi[1] + rssi[2] + rssi[3] + rssi[4])/5;
		aRssi[1] = (rssi[1] + rssi[2] + rssi[3] + rssi[4] + rssi[5])/5;
		aRssi[2] = (rssi[2] + rssi[3] + rssi[4] + rssi[5] + rssi[6])/5;
	} else {
		aRssi[0] = (rssi[0] + rssi[1] + rssi[2])/3;
		aRssi[1] = (rssi[1] + rssi[2] + rssi[3])/3;
		aRssi[2] = (rssi[2] + rssi[3] + rssi[4])/3;
	}

	for (i = 0; i < 3; i++) {
		if (aRssi[i] > mRssi) {
			mRssi = aRssi[i];
			pos = i; 
		}
	}

	if (use_new_search==0) {	
		if (sz == 7) {
			start_ocond = (aRssi[1] >= (wrelative_th + rssi[0]));
			end_ocond = (aRssi[1] >= (wrelative_th + rssi[6]));
			start_cond = ((*(rssi+3)) >= (*(rssi+1)+relative_th));
			end_cond = ((*(rssi+3)) >= (*(rssi+5)+relative_th));
			if (start_cond)
				start_cond=((*(rssi+2)) >= (*(rssi+1)+relative_th_side));
			if (end_cond)
				end_cond = ((*(rssi+4)) >= (*(rssi+5)+relative_th_side));
			least_cond = (*(rssi+3) )>= least_th;
		} else {
			start_ocond = (aRssi[pos] >= (wrelative_th + rssi[0]));
			end_ocond = (aRssi[pos] >= (wrelative_th + rssi[4]));
			start_cond = ((*(rssi+2)) >= (*(rssi+1)+relative_th));
			end_cond = ((*(rssi+2)) >= (*(rssi+3)+relative_th));
			least_cond = (*(rssi+2) )>= least_th;
		}
		if ((pos == 1) && start_cond && end_cond && least_cond && start_ocond && end_ocond)
			found = 1;
	} else {
		if (sz == 7) {
			start_ocond = 0;
			if (((*(overth+3))*relative_ratio_th) <= (*(overth+1)))
				if (((*(overth+3))*relative_ratio_th) <= (*(overth+2)))
					start_ocond=1;
			end_ocond = 0;
			if 	(((*(overth+3))*relative_ratio_th) <= (*(overth+5)))
				if (((*(overth+3))*relative_ratio_th) <= (*(overth+4)))
					end_ocond=1;
			start_cond = ((*(rssi+3)) >= (*(rssi+1)+relative_th));
			end_cond = ((*(rssi+3)) >= (*(rssi+5)+relative_th));
			least_cond = (*(rssi+3) )>= least_th;

			pos=1;
		} else {
			start_ocond = ((*(overth+2))*relative_ratio_th) <= (*(overth+1));
			end_ocond =   ((*(overth+2))*relative_ratio_th) <= (*(overth+3));
			start_cond = ((*(rssi+2)) >= (*(rssi+1)+relative_th));
			end_cond = ((*(rssi+2)) >= (*(rssi+3)+relative_th));
			least_cond = (*(rssi+2) )>= least_th;
			pos=1;
		}

		if ((pos == 1) && start_cond && end_cond && least_cond && start_ocond && end_ocond)
			found = 1;
	}

	return found;
}

static int atv_get_buserror()
{
	return chip.buserror;
}



static void atv_vtbl_init(tNmiAtvVtbl *ptv)
{
	ptv->init							= atv_chip_init;
 	ptv->r32							= rReg32;
	ptv->w32						= wReg32;
	ptv->rfr							= atvrfread;
	ptv->rfw							= atvrfwrite;
	ptv->tune						= atv_tuner_config;
	ptv->scan						= atv_scan;
	ptv->getchipid				= atv_get_chipid;
	ptv->reset						= atv_demod_reset;
	ptv->checklock				= atv_demod_check_lock;
	ptv->getfreqoffset			= atv_demod_freq_offset;
	ptv->gettimeoffset			= atv_demod_time_offset;
	ptv->getfedagc				= atv_fe_agc_gain;
	ptv->getfeagclock			= atv_fe_agc_lock;
	ptv->getfecplllock			= atv_fe_lock;
	ptv->getvdecsync			= atv_vdec_sync_status;
	ptv->getvdecblolock		= atv_vdec_blo_lock;
	ptv->getvdeclinewidth	= atv_vdec_get_avg_line_width_dev;
	ptv->getvdecnoise			= atv_vdec_get_noise_var;
	ptv->trackuvgain			= atv_vdec_adj_uv_gain;
	ptv->setuvgain				= atv_vdec_set_uv_gain;
	ptv->setuvsnrth				= atv_vdec_set_uv_snr_threshold;
	ptv->setuviir					= atv_vdec_set_uv_avg_factor;
	ptv->getadecagclock		= atv_adec_agc_lock;
	ptv->getadeclock			= atv_adec_lock;
	ptv->getadecagcgain		= atv_adec_agc_gain;
	ptv->getadecstatus		= atv_adec_get_status;
	ptv->play						= atv_fpga_play;
	ptv->mute						= atv_adec_mute;
	ptv->getvdecsnr			= atv_vdec_get_snr;
	ptv->getrssi					= atv_adec_get_rssi;
	ptv->getoverthcount    =atv_get_overthcount;
	ptv->setvolume				= atv_adec_adj_volume;
	ptv->forcecolor				= atv_be_force_color;
	ptv->resetstart				= atv_demod_reset_start;
	ptv->getspur					= atv_get_spur_offset;
	ptv->showpattern		= atv_show_pattern;
	ptv->applyoffset		= atv_apply_spur_offset;
	ptv->fmavgrssi			= fm_get_window_rssi;
	ptv->fmscanlist			= fm_select_channel_list;
	ptv->fmseek				= fm_seek_select;
	ptv->setscanlevel		= atv_set_scan_level;
	ptv->setaudiofilter		= atv_adec_set_filter;
	ptv->sleep					= atv_sleep;
	ptv->notchctl				= atv_notch_set;
	ptv->notchchk			= atv_notch_check;
	ptv->getrfgain			= atv_rf_agc_gain;
	ptv->setsize				=	atv_bt656_set_size;
	ptv->getferssi			= atv_fe_get_rssi;
	ptv->getfbstatus		= atv_demod_fb_get_status;
	ptv->setlumagain		= atv_vdec_set_luma_gain;
	ptv->getplllevel			= atv_demod_get_pll_level;
	ptv->setminscale		= atv_vdec_set_min_scale;
	ptv->setfastscan		= atv_set_fast_scan;
	ptv->getnotchfreq		= atv_get_notch_frequency;
	ptv->setfieldoutput	= atv_be_set_field_map;
	ptv->uselocalpeak		= atv_fe_set_agc_peak;
	ptv->setnotch				= atv_notch_spur;
	ptv->setgamma			= atv_be_gamma_correction;
	ptv->setlumaoffset	= atv_vdec_set_luma_offset;
	ptv->trackdoppler			= atv_vdec_track_doppler;
	ptv->setfmscanth		= fm_set_scan_level;
	ptv->setchswitchspeed = atv_vdec_set_ch_switch_speed;
	ptv->setlinedelay = atv_be_line_delay;
	ptv->acictl = atv_fe_aci_ctl;
	ptv->setscansnrlevel = atv_set_scan_snr_level;
	ptv->getbuserror		= atv_get_buserror;
}


#endif


void nmi_common_init(tNmiIn *inp, tNmiAtvVtbl *ptv)
{
	memset((void *)&chip, 0, sizeof(tNmiAsic));
	chip.priv.standard = -1;
	

	if (inp != NULL)
		memcpy((void *)&chip.inp, (void *)inp, sizeof(tNmiIn));

	if (ptv != NULL) {
		atv_vtbl_init(ptv);
	}
	
	return;
}



#endif

/*lint -restore*/








