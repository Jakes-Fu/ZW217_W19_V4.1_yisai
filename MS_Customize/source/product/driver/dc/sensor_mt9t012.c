#include "sensor_cfg.h"
#include "sensor_drv.h"
//#include "sc6800_reg.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"

#define MT9T012_I2C_ADDR_W    	0x20
#define MT9T012_I2C_ADDR_R    	0x21

#define SENSOR_GAIN_SCALE		16

#define MT9T012_PREVIEW_LINELENGTH 2250 //0x960 
#define MT9T012_CAPTURE_LINELENGTH 0xAAC
#define MT9T012_PREVIEW_PCLK  36000000
#define MT9T012_CAPTURE_PCLK  36000000
#define MT9T012_PREVIEW_FRAMELENGTH  800//1000

//#define CARE_CAPTURE_FRAMERATE

#define MT9T012_TRACE_DEBUG(...)



//LOCAL uint32 MT9T012_WriteReg(uint32 param);
LOCAL uint32 MT9T012_ReadReg(uint32 param);
LOCAL uint32 set_ae_enable(uint32 enable);
LOCAL uint32 set_hmirror_enable(uint32 enable);
LOCAL uint32 set_vmirror_enable(uint32 enable);
LOCAL uint32 set_preview_mode(uint32 preview_mode);
LOCAL uint32 MT9T012_Identify(uint32 param);
LOCAL uint32 MT9T012_BeforeSnapshot(uint32 param);

#if 0
LOCAL uint32 set_brightness(uint32 level);
LOCAL uint32 set_contrast(uint32 level);
LOCAL uint32 set_sharpness(uint32 level);
LOCAL uint32 set_saturation(uint32 level);
#endif
LOCAL uint32 set_image_effect(uint32 effect_type);


LOCAL uint32 read_ev_value(uint32 value);
LOCAL uint32 write_ev_value(uint32 exposure_value);
LOCAL uint32 read_gain_value(uint32 value);
LOCAL uint32 write_gain_value(uint32 gain_value);
LOCAL uint32 read_gain_scale(uint32 value);
LOCAL uint32 set_frame_rate(uint32 param);
LOCAL uint32 MT9T012_AfterSnapshot(uint32 param);

//LOCAL uint32 s_preview_mode;
LOCAL uint8 s_GainReg;
LOCAL uint8 s_ExpReg_high;
LOCAL uint8 s_ExpReg_low;


__align(4)const SENSOR_REG_T MT9T012_RGB_COMMON[]=
{

	  
	{0x0103 , 0x01},
	{SENSOR_WRITE_DELAY,50},
	{0x301a , 0x18},	
	{0x301b , 0xcc},



	{0x0202 , 0x05},  //intergration time

	/*
	{0x0300 , 0x00},  //clock 
	{0x0301 , 0x05},
	{0x0302 , 0x00},
	{0x0303 , 0x02},
	{0x0304 , 0x00},
	{0x0305 , 0x02},
	{0x0306 , 0x00},
	{0x0307 , 0x80},//clock end

	{0x0383 , 0x03},  //x_odd_inc	
	{0x0387 , 0x03},  //y_odd_inc		

	{0x0348 , 0x08},/////// addr 
	{0x0349 , 0x01},	
	{0x034a , 0x06},
	{0x034b , 0x01},
	*/	
	{0x034c , 0x04},//image size
	{0x034d , 0x00},
	{0x034e , 0x03},
	{0x034f , 0x00},

	
//lens correction
#if 1
{0x3600, 0x51}, //LENS_CORRECTION_CONTROL
{0x3601, 0xA9},
{0x3602, 0x20}, //ZONE_BOUNDS_X1_X2
{0x3603, 0x30}, //ZONE_BOUNDS_X1_X2
{0x3604, 0x50}, //ZONE_BOUNDS_X0_X3
{0x3605, 0x10}, //ZONE_BOUNDS_X0_X3
{0x3606, 0x70}, //ZONE_BOUNDS_X4_X5
{0x3607, 0x60}, //ZONE_BOUNDS_X4_X5
{0x3608, 0x18}, //ZONE_BOUNDS_Y1_Y2
{0x3609, 0x24}, //ZONE_BOUNDS_Y1_Y2
{0x360A, 0x3c}, //ZONE_BOUNDS_Y0_Y3
{0x360b, 0x0c}, //ZONE_BOUNDS_Y0_Y3
{0x360C, 0x54}, //ZONE_BOUNDS_Y4_Y5
{0x360d, 0x48}, //ZONE_BOUNDS_Y4_Y5

{0x360E, 0x30}, //CENTER_OFFSET
{0x360F, 0x40}, //CENTER_OFFSET

{0x3610, 0x00}, //FX_RED
{0x3611, 0x94}, //FX_RED
{0x3618, 0x00}, //FY_RED
{0x3619, 0xb3}, //FY_RED

{0x3620, 0x0D}, //DF_DX_RED
{0x3621, 0x7F}, //DF_DX_RED
{0x3628, 0x09}, //DF_DY_RED
{0x3629, 0x1B}, //DF_DY_RED

{0x3632, 0x47}, //SECOND_DERIV_ZONE_0_RED
{0x3633, 0x19}, //SECOND_DERIV_ZONE_0_RED
{0x363A, 0x37}, //SECOND_DERIV_ZONE_1_RED
{0x363B, 0x0C}, //SECOND_DERIV_ZONE_1_RED
{0x3642, 0x4D}, //SECOND_DERIV_ZONE_2_RED
{0x3643, 0x1F}, //SECOND_DERIV_ZONE_2_RED
{0x364A, 0x61}, //SECOND_DERIV_ZONE_3_RED
{0x364b, 0x27}, //SECOND_DERIV_ZONE_3_RED
{0x3652, 0x4A}, //SECOND_DERIV_ZONE_4_RED
{0x3653, 0x33}, //SECOND_DERIV_ZONE_4_RED
{0x365A, 0x60}, //SECOND_DERIV_ZONE_5_RED
{0x365b, 0x22}, //SECOND_DERIV_ZONE_5_RED
{0x3662, 0x3D}, //SECOND_DERIV_ZONE_6_RED
{0x3663, 0x1D}, //SECOND_DERIV_ZONE_6_RED
{0x366A, 0x23}, //SECOND_DERIV_ZONE_7_RED
{0x366b, 0x12}, //SECOND_DERIV_ZONE_7_RED

{0x3686, 0x04}, //K_FACTOR_IN_K_FX_FY_R_TL
{0x3687, 0x68}, //K_FACTOR_IN_K_FX_FY_R_TL
{0x368E, 0x00}, //K_FACTOR_IN_K_FX_FY_R_TR
{0x368f, 0x10} ,//K_FACTOR_IN_K_FX_FY_R_TR
{0x3676, 0x00}, //K_FACTOR_IN_K_FX_FY_R_BL
{0x3677, 0x10}, //K_FACTOR_IN_K_FX_FY_R_BL
{0x367E, 0x04}, //K_FACTOR_IN_K_FX_FY_R_BR
{0x367f, 0x28} ,//K_FACTOR_IN_K_FX_FY_R_BR


{0x3612, 0x00}, //FX_GREEN
{0x3613, 0xF6}, //FX_GREEN
{0x361A, 0x00}, //FY_GREEN
{0x361B, 0xAE}, //FY_GREEN

{0x3622, 0x0D}, //DF_DX_GREEN
{0x3623, 0x3A}, //DF_DX_GREEN
{0x362A, 0x0A}, //DF_DY_GREEN
{0x362B, 0x6E}, //DF_DY_GREEN

{0x3634, 0x0C}, //SECOND_DERIV_ZONE_0_GREEN
{0x3635, 0xFB}, //SECOND_DERIV_ZONE_0_GREEN
{0x363C, 0x31}, //SECOND_DERIV_ZONE_1_GREEN
{0x363D, 0x13}, //SECOND_DERIV_ZONE_1_GREEN
{0x3644, 0x55}, //SECOND_DERIV_ZONE_2_GREEN
{0x3645, 0x27}, //SECOND_DERIV_ZONE_2_GREEN
{0x364C, 0x62}, //SECOND_DERIV_ZONE_3_GREEN
{0x364D, 0x31}, //SECOND_DERIV_ZONE_3_GREEN
{0x3654, 0x55}, //SECOND_DERIV_ZONE_4_GREEN
{0x3655, 0x2F}, //SECOND_DERIV_ZONE_4_GREEN
{0x365C, 0x54}, //SECOND_DERIV_ZONE_5_GREEN
{0x365D, 0x27}, //SECOND_DERIV_ZONE_5_GREEN
{0x3664, 0x39}, //SECOND_DERIV_ZONE_6_GREEN
{0x3665, 0x13}, //SECOND_DERIV_ZONE_6_GREEN
{0x366C, 0x34}, //SECOND_DERIV_ZONE_7_GREEN
{0x366D, 0xE9}, //SECOND_DERIV_ZONE_7_GREEN

{0x3688, 0x04} ,//K_FACTOR_IN_K_FX_FY_G1_TL
{0x3689, 0x28} ,//K_FACTOR_IN_K_FX_FY_G1_TL
{0x3690, 0x04} ,//K_FACTOR_IN_K_FX_FY_G1_TR
{0x3691, 0x28} ,//K_FACTOR_IN_K_FX_FY_G1_TR
{0x3678, 0x04} ,//K_FACTOR_IN_K_FX_FY_G1_BL
{0x3679, 0x20} ,//K_FACTOR_IN_K_FX_FY_G1_BL
{0x3680, 0x04} ,//K_FACTOR_IN_K_FX_FY_G1_BR
{0x3681, 0x10} ,//K_FACTOR_IN_K_FX_FY_G1_BR

{0x3616, 0x00} ,//FX_BLUE
{0x3617, 0x9E} ,//FX_BLUE
{0x361E, 0x00} ,//FY_BLUE
{0x361F, 0x96} ,//FY_BLUE
{0x3626, 0x0D} ,//DF_DX_BLUE
{0x3627, 0xAF} ,//DF_DX_BLUE
{0x362E, 0x0A}, //DF_DY_BLUE
{0x362F, 0x38}, //DF_DY_BLUE

{0x3638, 0x47} ,//SECOND_DERIV_ZONE_0_BLUE
{0x3639, 0x0A} ,//SECOND_DERIV_ZONE_0_BLUE
{0x3640, 0x23} ,//SECOND_DERIV_ZONE_1_BLUE
{0x3641, 0x16} ,//SECOND_DERIV_ZONE_1_BLUE
{0x3648, 0x3C} ,//SECOND_DERIV_ZONE_2_BLUE
{0x3649, 0x19} ,//SECOND_DERIV_ZONE_2_BLUE
{0x3650, 0x4A} ,//SECOND_DERIV_ZONE_3_BLUE
{0x3651, 0x26}, //SECOND_DERIV_ZONE_3_BLUE
{0x3658, 0x35}, //SECOND_DERIV_ZONE_4_BLUE
{0x3659, 0x22}, //SECOND_DERIV_ZONE_4_BLUE
{0x3660, 0x46} ,//SECOND_DERIV_ZONE_5_BLUE
{0x3661, 0x19} ,//SECOND_DERIV_ZONE_5_BLUE
{0x3668, 0x28}, //SECOND_DERIV_ZONE_6_BLUE
{0x3669, 0x11} ,//SECOND_DERIV_ZONE_6_BLUE
{0x3670, 0x4E} ,//SECOND_DERIV_ZONE_7_BLUE
{0x3671, 0x0D} ,//SECOND_DERIV_ZONE_7_BLUE

{0x368C, 0x04} ,//K_FACTOR_IN_K_FX_FY_B_TL
{0x368D, 0x80} ,//K_FACTOR_IN_K_FX_FY_B_TL
{0x3694, 0x00} ,//K_FACTOR_IN_K_FX_FY_B_TR
{0x3695, 0x10} ,//K_FACTOR_IN_K_FX_FY_B_TR
{0x367C, 0x00}, //K_FACTOR_IN_K_FX_FY_B_BL
{0x367D, 0x48}, //K_FACTOR_IN_K_FX_FY_B_BL
{0x3684, 0x04}, //K_FACTOR_IN_K_FX_FY_B_BR
{0x3685, 0x48} ,//K_FACTOR_IN_K_FX_FY_B_BR


{0x3614, 0x00} ,//FX_GREEN2
{0x3615, 0xD3} ,//FX_GREEN2
{0x361C, 0x00} ,//FY_GREEN2
{0x361D, 0xAB} ,//FY_GREEN2
{0x3624, 0x0D} ,//DF_DX_GREEN2
{0x3625, 0x4B} ,//DF_DX_GREEN2
{0x362C, 0x0A}, //DF_DY_GREEN2
{0x362D, 0x8C} ,//DF_DY_GREEN2

{0x3636, 0x0A} ,//SECOND_DERIV_ZONE_0_GREEN2
{0x3637, 0x08} ,//SECOND_DERIV_ZONE_0_GREEN2
{0x363E, 0x34} ,//SECOND_DERIV_ZONE_1_GREEN2
{0x363F, 0x0E} ,//SECOND_DERIV_ZONE_1_GREEN2
{0x3646, 0x4E} ,//SECOND_DERIV_ZONE_2_GREEN2
{0x3647, 0x25} ,//SECOND_DERIV_ZONE_2_GREEN2
{0x364E, 0x62} ,//SECOND_DERIV_ZONE_3_GREEN2
{0x364F, 0x2B} ,//SECOND_DERIV_ZONE_3_GREEN2
{0x3656, 0x5C} ,//SECOND_DERIV_ZONE_4_GREEN2
{0x3657, 0x34} ,//SECOND_DERIV_ZONE_4_GREEN2
{0x365E, 0x63} ,//SECOND_DERIV_ZONE_5_GREEN2
{0x365F, 0X2A} ,//SECOND_DERIV_ZONE_5_GREEN2
{0x3666, 0x32} ,//SECOND_DERIV_ZONE_6_GREEN2
{0x3667, 0x13} ,//SECOND_DERIV_ZONE_6_GREEN2
{0x366E, 0x55} ,//SECOND_DERIV_ZONE_7_GREEN2
{0x366F, 0xF4} ,//SECOND_DERIV_ZONE_7_GREEN2

{0x368A, 0x04} ,//K_FACTOR_IN_K_FX_FY_G2_TL
{0x368B, 0x20} ,//K_FACTOR_IN_K_FX_FY_G2_TL
{0x3692, 0x04} ,//K_FACTOR_IN_K_FX_FY_G2_TR
{0x3693, 0x20} ,//K_FACTOR_IN_K_FX_FY_G2_TR
{0x367A, 0x04} ,//K_FACTOR_IN_K_FX_FY_G2_BL
{0x367B, 0x30} ,//K_FACTOR_IN_K_FX_FY_G2_BL
{0x3682, 0x04} ,//K_FACTOR_IN_K_FX_FY_G2_BR
{0x3683, 0x08} ,//K_FACTOR_IN_K_FX_FY_G2_BR

{0x3672, 0x00},//X2_FACTORS
{0x3673, 0x00},//X2_FACTOR2S
{0x3674, 0x00},//GLOBAL_OFFSET_FXY_FUNCTION
{0x3675, 0x20},//GLOBAL_OFFSET_FXY_FUNCTION
{0x3696, 0x00},//CORNER_FACTOR_CONTROL
{0x3697, 0x28},//CORNER_FACTOR_CONTROL
//STATE=Lens Correction Falloff, 85
//STATE=Lens Correction Center X, 956
//STATE=Lens Correction Center Y, 884


{0x318A, 0x80},//ENABLE LENS CORRECTION
{0x318B, 0x00},//ENABLE LENS CORRECTION2
#else
{0x3600, 0x51}, //LENS_CORRECTION_CONTROL
{0x3601, 0x68},
{0x3602, 0x1E}, //ZONE_BOUNDS_X1_X2
{0x3603, 0x2C}, //ZONE_BOUNDS_X1_X2
{0x3604, 0x4C}, //ZONE_BOUNDS_X0_X3
{0x3605, 0x0E}, //ZONE_BOUNDS_X0_X3
{0x3606, 0x6E}, //ZONE_BOUNDS_X4_X5
{0x3607, 0x5E}, //ZONE_BOUNDS_X4_X5
{0x3608, 0x1C}, //ZONE_BOUNDS_Y1_Y2
{0x3609, 0x2A}, //ZONE_BOUNDS_Y1_Y2
{0x360A, 0x42}, //ZONE_BOUNDS_Y0_Y3
{0x360b, 0x0E}, //ZONE_BOUNDS_Y0_Y3
{0x360C, 0x56}, //ZONE_BOUNDS_Y4_Y5
{0x360d, 0x4C}, //ZONE_BOUNDS_Y4_Y5
{0x360E, 0x38}, //CENTER_OFFSET
{0x360F, 0x3C}, //CENTER_OFFSET
{0x3610, 0x00}, //FX_RED
{0x3611, 0xDD}, //FX_RED
{0x3618, 0x01}, //FY_RED
{0x3619, 0x6C}, //FY_RED
{0x3620, 0x0C}, //DF_DX_RED
{0x3621, 0x56}, //DF_DX_RED
{0x3628, 0x08}, //DF_DY_RED
{0x3629, 0xB0}, //DF_DY_RED
{0x3632, 0x68}, //SECOND_DERIV_ZONE_0_RED
{0x3633, 0x1E}, //SECOND_DERIV_ZONE_0_RED
{0x363A, 0x35}, //SECOND_DERIV_ZONE_1_RED
{0x363B, 0x21}, //SECOND_DERIV_ZONE_1_RED

{0x3642, 0x31}, //SECOND_DERIV_ZONE_2_RED
{0x3643, 0x1A}, //SECOND_DERIV_ZONE_2_RED
{0x364A, 0x37}, //SECOND_DERIV_ZONE_3_RED
{0x364b, 0x31}, //SECOND_DERIV_ZONE_3_RED
{0x3652, 0x33}, //SECOND_DERIV_ZONE_4_RED
{0x3653, 0x30}, //SECOND_DERIV_ZONE_4_RED
{0x365A, 0x36}, //SECOND_DERIV_ZONE_5_RED
{0x365b, 0x2F}, //SECOND_DERIV_ZONE_5_RED
{0x3662, 0x1D}, //SECOND_DERIV_ZONE_6_RED
{0x3663, 0x46}, //SECOND_DERIV_ZONE_6_RED
{0x366A, 0x3A}, //SECOND_DERIV_ZONE_7_RED
{0x366b, 0x28}, //SECOND_DERIV_ZONE_7_RED
{0x3686, 0x00}, //K_FACTOR_IN_K_FX_FY_R_TL
{0x3687, 0x10}, //K_FACTOR_IN_K_FX_FY_R_TL
{0x368E, 0x00}, //K_FACTOR_IN_K_FX_FY_R_TR
{0x368f, 0x08} ,//K_FACTOR_IN_K_FX_FY_R_TR
{0x3676, 0x04}, //K_FACTOR_IN_K_FX_FY_R_BL
{0x3677, 0x80}, //K_FACTOR_IN_K_FX_FY_R_BL
{0x367E, 0x00}, //K_FACTOR_IN_K_FX_FY_R_BR
{0x367f, 0x08} ,//K_FACTOR_IN_K_FX_FY_R_BR


{0x3612, 0x01}, //FX_GREEN
{0x3613, 0x42}, //FX_GREEN
{0x361A, 0x01}, //FY_GREEN
{0x361B, 0x4F}, //FY_GREEN
{0x3622, 0x0C}, //DF_DX_GREEN
{0x3623, 0x06}, //DF_DX_GREEN
{0x362A, 0x0A}, //DF_DY_GREEN
{0x362B, 0x8A}, //DF_DY_GREEN
{0x3634, 0x2D}, //SECOND_DERIV_ZONE_0_GREEN
{0x3635, 0x06}, //SECOND_DERIV_ZONE_0_GREEN
{0x363C, 0x23}, //SECOND_DERIV_ZONE_1_GREEN
{0x363D, 0x1A}, //SECOND_DERIV_ZONE_1_GREEN
{0x3644, 0x3E}, //SECOND_DERIV_ZONE_2_GREEN
{0x3645, 0x26}, //SECOND_DERIV_ZONE_2_GREEN
{0x364C, 0x36}, //SECOND_DERIV_ZONE_3_GREEN
{0x364D, 0x3E}, //SECOND_DERIV_ZONE_3_GREEN
{0x3654, 0x3B}, //SECOND_DERIV_ZONE_4_GREEN
{0x3655, 0x38}, //SECOND_DERIV_ZONE_4_GREEN
{0x365C, 0x37},//SECOND_DERIV_ZONE_5_GREEN
{0x365D, 0x3B}, //SECOND_DERIV_ZONE_5_GREEN
{0x3664, 0x16}, //SECOND_DERIV_ZONE_6_GREEN
{0x3665, 0x29}, //SECOND_DERIV_ZONE_6_GREEN
{0x366C, 0x3A}, //SECOND_DERIV_ZONE_7_GREEN
{0x366D, 0x26}, //SECOND_DERIV_ZONE_7_GREEN
{0x3688, 0x00} ,//K_FACTOR_IN_K_FX_FY_G1_TL
{0x3689, 0x18} ,//K_FACTOR_IN_K_FX_FY_G1_TL
{0x3690, 0x04} ,//K_FACTOR_IN_K_FX_FY_G1_TR
{0x3691, 0x18} ,//K_FACTOR_IN_K_FX_FY_G1_TR
{0x3678, 0x04} ,//K_FACTOR_IN_K_FX_FY_G1_BL
{0x3679, 0x78} ,//K_FACTOR_IN_K_FX_FY_G1_BL
{0x3680, 0x00} ,//K_FACTOR_IN_K_FX_FY_G1_BR
{0x3681, 0x28} ,//K_FACTOR_IN_K_FX_FY_G1_BR
{0x3616, 0x00} ,//FX_BLUE
{0x3617, 0xF2} ,//FX_BLUE
{0x361E, 0x01} ,//FY_BLUE
{0x361F, 0x11} ,//FY_BLUE
{0x3626, 0x0C} ,//DF_DX_BLUE
{0x3627, 0x03} ,//DF_DX_BLUE
{0x362E, 0x0B}, //DF_DY_BLUE
{0x362F, 0x17}, //DF_DY_BLUE

{0x3638, 0x3D} ,//SECOND_DERIV_ZONE_0_BLUE
{0x3639, 0x23} ,//SECOND_DERIV_ZONE_0_BLUE
{0x3640, 0x1B} ,//SECOND_DERIV_ZONE_1_BLUE
{0x3641, 0x24} ,//SECOND_DERIV_ZONE_1_BLUE
{0x3648, 0x2B} ,//SECOND_DERIV_ZONE_2_BLUE
{0x3649, 0x18} ,//SECOND_DERIV_ZONE_2_BLUE
{0x3650, 0x28} ,//SECOND_DERIV_ZONE_3_BLUE
{0x3651, 0x2D}, //SECOND_DERIV_ZONE_3_BLUE
{0x3658, 0x23}, //SECOND_DERIV_ZONE_4_BLUE
{0x3659, 0x27}, //SECOND_DERIV_ZONE_4_BLUE
{0x3660, 0x36} ,//SECOND_DERIV_ZONE_5_BLUE
{0x3661, 0x26} ,//SECOND_DERIV_ZONE_5_BLUE
{0x3668, 0xFA}, //SECOND_DERIV_ZONE_6_BLUE
{0x3669, 0x27} ,//SECOND_DERIV_ZONE_6_BLUE
{0x3670, 0x35} ,//SECOND_DERIV_ZONE_7_BLUE
{0x3671, 0x14} ,//SECOND_DERIV_ZONE_7_BLUE

{0x368C, 0x00} ,//K_FACTOR_IN_K_FX_FY_B_TL
{0x368D, 0x10} ,//K_FACTOR_IN_K_FX_FY_B_TL
{0x3694, 0x04} ,//K_FACTOR_IN_K_FX_FY_B_TR
{0x3695, 0x00} ,//K_FACTOR_IN_K_FX_FY_B_TR
{0x367C, 0x04}, //K_FACTOR_IN_K_FX_FY_B_BL
{0x367D, 0x48}, //K_FACTOR_IN_K_FX_FY_B_BL
{0x3684, 0x04}, //K_FACTOR_IN_K_FX_FY_B_BR
{0x3685, 0x00} ,//K_FACTOR_IN_K_FX_FY_B_BR


{0x3614, 0x01} ,//FX_GREEN2
{0x3615, 0x1E} ,//FX_GREEN2
{0x361C, 0x01} ,//FY_GREEN2
{0x361D, 0x40} ,//FY_GREEN2
{0x3624, 0x0C} ,//DF_DX_GREEN2
{0x3625, 0x1F} ,//DF_DX_GREEN2
{0x362C, 0x0A}, //DF_DY_GREEN2
{0x362D, 0xA2} ,//DF_DY_GREEN2

{0x3636, 0x30} ,//SECOND_DERIV_ZONE_0_GREEN2
{0x3637, 0x13} ,//SECOND_DERIV_ZONE_0_GREEN2
{0x363E, 0x25} ,//SECOND_DERIV_ZONE_1_GREEN2
{0x363F, 0x16} ,//SECOND_DERIV_ZONE_1_GREEN2
{0x3646, 0x37} ,//SECOND_DERIV_ZONE_2_GREEN2
{0x3647, 0x25} ,//SECOND_DERIV_ZONE_2_GREEN2
{0x364E, 0x36} ,//SECOND_DERIV_ZONE_3_GREEN2
{0x364F, 0x37} ,//SECOND_DERIV_ZONE_3_GREEN2
{0x3656, 0x3D} ,//SECOND_DERIV_ZONE_4_GREEN2
{0x3657, 0x3A} ,//SECOND_DERIV_ZONE_4_GREEN2
{0x365E, 0x36} ,//SECOND_DERIV_ZONE_5_GREEN2
{0x365F, 0x3B} ,//SECOND_DERIV_ZONE_5_GREEN2
{0x3666, 0x1D} ,//SECOND_DERIV_ZONE_6_GREEN2
{0x3667, 0x29} ,//SECOND_DERIV_ZONE_6_GREEN2
{0x366E, 0x47} ,//SECOND_DERIV_ZONE_7_GREEN2
{0x366F, 0x21} ,//SECOND_DERIV_ZONE_7_GREEN2
{0x368A, 0x00} ,//K_FACTOR_IN_K_FX_FY_G2_TL
{0x368B, 0x18} ,//K_FACTOR_IN_K_FX_FY_G2_TL
{0x3692, 0x04} ,//K_FACTOR_IN_K_FX_FY_G2_TR
{0x3693, 0x18} ,//K_FACTOR_IN_K_FX_FY_G2_TR
{0x367A, 0x04} ,//K_FACTOR_IN_K_FX_FY_G2_BL
{0x367B, 0x80} ,//K_FACTOR_IN_K_FX_FY_G2_BL
{0x3682, 0x00} ,//K_FACTOR_IN_K_FX_FY_G2_BR
{0x3683, 0x18} ,//K_FACTOR_IN_K_FX_FY_G2_BR
{0x3672, 0x00},//X2_FACTORS
{0x3673, 0x00},//X2_FACTOR2S
{0x3674, 0x00},//GLOBAL_OFFSET_FXY_FUNCTION
{0x3675, 0x20},//GLOBAL_OFFSET_FXY_FUNCTION
{0x3696, 0x00},//CORNER_FACTOR_CONTROL
{0x3697, 0x28},//CORNER_FACTOR_CONTROL
//STATE=Lens Correction Falloff, 85
//STATE=Lens Correction Center X, 956
//STATE=Lens Correction Center Y, 884


{0x318A, 0x80},//ENABLE LENS CORRECTION
{0x318B, 0x00},//ENABLE LENS CORRECTION2


#endif


	{0x100, 0x01},  //Mode_Select  

	
};

__align(4)const SENSOR_REG_T MT9T012_RGB_1024X768[] =
{
#if 1	


//{0x0103, 0x01 }, // SOFTWARE_RESET
{0x301A, 0x18 }, // RESET_REGISTER
{0x301B, 0xC4 }, // RESET_REGISTER

{0x0100, 0x00 }, // MODE_SELECT
{0x0104, 0x01 }, // GROUPED_PARAMETER_HOLD

	{0x0300 , 0x00},  //clock 
	{0x0301 , 0x05},
	{0x0302 , 0x00},
	{0x0303 , 0x02},
	{0x0304 , 0x00},
	{0x0305 , 0x02},
	{0x0306 , 0x00},
	{0x0307 , 60},//clock end   //20:pclk = mclk ; 40 : pclk =  2* mclk

{0x1148, 0x04 }, // MIN_LINE_BLANKING_PCK
{0x1149, 0x68 }, // MIN_LINE_BLANKING_PCK
{0x1144, 0x05 }, // MIN_LINE_LENGTH_PCK
{0x1145, 0xFD }, // MIN_LINE_LENGTH_PCK
{0x1008, 0x03 }, // FINE_INTEGRATION_TIME_MIN
{0x1009, 0xE5 }, // FINE_INTEGRATION_TIME_MIN
{0x100A, 0x02 }, // FINE_INTEGRATION_TIME_MAX_MARGIN
{0x100B, 0x15 }, // FINE_INTEGRATION_TIME_MAX_MARGIN
{0x300E, 0x03 }, // SAMPLE_TIME_PCK
{0x300F, 0xCD }, // SAMPLE_TIME_PCK
{0x3010, 0x02 }, // FINE_CORRECTION
{0x3011, 0x15 }, // FINE_CORRECTION
{0x3014, 0x03 }, // FINE_INTEGRATION_TIME_
{0x3015, 0xE5 }, // FINE_INTEGRATION_TIME_
{0x3044, 0x05 }, // DARK_CONTROL
{0x3045, 0x41 }, // DARK_CONTROL


{0x0344, 0x00 }, // X_ADDR_START
{0x0345, 0x04 }, // X_ADDR_START
{0x0346, 0x00 }, // Y_ADDR_START
{0x0347, 0x04 }, // Y_ADDR_START
{0x0348, 0x08 }, // X_ADDR_END
{0x0349, 0x01 }, // X_ADDR_END
{0x034A, 0x06 }, // Y_ADDR_END
{0x034B, 0x01 }, // Y_ADDR_END
{0x0382, 0x00 }, // X_ODD_INC
{0x0383, 0x03 }, // X_ODD_INC
{0x0386, 0x00 }, // Y_ODD_INC
{0x0387, 0x03 }, // Y_ODD_INC
{0x3040, 0x04 }, // READ_MODE
{0x3041, 0x6C }, // READ_MODE
{0x034C, 0x04 }, // X_OUTPUT_SIZE
{0x034D, 0x00 }, // X_OUTPUT_SIZE
{0x034E, 0x03 }, // Y_OUTPUT_SIZE
{0x034F, 0x00 }, // Y_OUTPUT_SIZE
{0x0340, 0x03 }, // FRAME_LENGTH_LINES
{0x0341, 0x10 }, // FRAME_LENGTH_LINES

{0x0342, 0x08 }, // LINE_LENGTH_PCK
{0x0343, 0xca }, // LINE_LENGTH_PCK
{0x0104, 0x00 }, // GROUPED_PARAMETER_HOLD
{0x0100, 0x01 }, // MODE_SELECT
#else


	{0x0100 , 0x00},  //streaming
	//{0x0103 , 0x01},
{0x104 , 1},
	{0x3040, 0x04 },// READ_MODE   //binning
	{0x3041, 0x6C },// READ_MODE

	{0x301a , 0x18},	
	{0x301b , 0xc4},

	{0x0100 , 0x00},  //streaming

	{0x0202 , 0x02},  //intergration time
	{0x0203 , 0xff},  //intergration time
	//{0x0205 , 0x30},  //SMIA Global gain
	
	{0x0300 , 0x00},  //clock 
	{0x0301 , 0x05},
	{0x0302 , 0x00},
	{0x0303 , 0x02},
	{0x0304 , 0x00},
	{0x0305 , 0x02},
	{0x0306 , 0x00},
	{0x0307 , 80},//clock end   //20:pclk = mclk ; 40 : pclk =  2* mclk


	{0x0383 , 0x03},  //x_odd_inc	
	{0x0387 , 0x03},  //y_odd_inc		

	//{0x0401 , 2},
	//{0x0405 , 25},  //scaling 19/16
#if 1  //30fps
	{0x0340, 0x03},
	{0x0341, 0x10},
	{0x0342, 0x08},
	{0x0343, 0x68},

#else  //36.6 max
	{0x0340, 0x01},
	{0x0341, 0xef},
	{0x0342, 0x05},
	{0x0343, 0x2c},

#endif
	// {0x601 , 0x02},   //test pattern

	{0x0344, 0x00 },// X_ADDR_START
	{0x0345, 0x04 },// X_ADDR_START
	{0x0346, 0x00 },// Y_ADDR_START
	{0x0347, 0x04 },// Y_ADDR_START

	{0x0348, 0x08 },// X_ADDR_END
	{0x0349, 0x01 },// X_ADDR_END
	{0x034A, 0x06 },// Y_ADDR_END
	{0x034b, 0x01 },// Y_ADDR_END

	{0x034c , 0x04},//image size
	{0x034d , 0x00},
	{0x034e , 0x03},
	{0x034f , 0x00},

	{0x305e , 0x1}, //0xff = (1+1) * (128/16) = 16  //Micron global gain
	{0x305f , 0xfe}, //0xff = (1+1) * (128/16) = 16  //Micron global gain

	{0x104 ,0},
#endif
	{0x100, 0x01},  //Mode_Select  

{SENSOR_WRITE_DELAY,50},
 //{0x601 , 0x02},   //test pattern
};

__align(4)const SENSOR_REG_T MT9T012_RGB_2048X1536[] =
{
#if 1
	
	//{0x301A, 0x18},  //0X18 IS okReset Register	
	//{0x301B, 0xcC},  //0XCC  //0xcc
	
	{0x100, 0x00},  //mode seletct
	//{0x0103 , 0x01},
	//{0x0103, 0x01},  //software reset
	{0x104 , 1},
	


       {0x301a , 0x18},	
	{0x301b , 0xcc},
	{0x0300 , 0x00},  //clock 
	{0x0301 , 0x05},
	{0x0302 , 0x00},
	{0x0303 , 0x02},
	{0x0304 , 0x00},
	{0x0305 , 0x02},
	{0x0306 , 0x00},
	{0x0307 , 60},//clock end   //20:pclk = mclk ; 40 : pclk =  2* mclk


	{0x301A, 0x18 }, // RESET_REGISTER
	{0x301B, 0xC4 }, // RESET_REGISTER

	{0x1148, 0x02 }, // MIN_LINE_BLANKING_PCK
	{0x1149, 0xAC }, // MIN_LINE_BLANKING_PCK
	{0x1144, 0x03 }, // MIN_LINE_LENGTH_PCK
	{0x1145, 0xA8 }, // MIN_LINE_LENGTH_PCK
	{0x1008, 0x02 }, // FINE_INTEGRATION_TIME_MIN
	{0x1009, 0x04 }, // FINE_INTEGRATION_TIME_MIN
	{0x100A, 0x01 }, // FINE_INTEGRATION_TIME_MAX_MARGIN
	{0x100B, 0x10 }, // FINE_INTEGRATION_TIME_MAX_MARGIN

       {0x300E, 0x01 }, // SAMPLE_TIME_PCK
	{0x300F, 0xEC }, // SAMPLE_TIME_PCK
	{0x3010, 0x01 }, // FINE_CORRECTION
	{0x3011, 0x10 }, // FINE_CORRECTION
/*	{0x3014, 0x03 }, // FINE_INTEGRATION_TIME_
	{0x3015, 0xE5 }, // FINE_INTEGRATION_TIME_
	{0x3044, 0x05 }, // DARK_CONTROL
	{0x3045, 0x41 }, // DARK_CONTROL

	//{0x202, 0x04},  //coarse intergration time 
*/
	{0x3040, 0x00 },// READ_MODE   //binning
	{0x3041, 0x6c },// READ_MODE

	{0x0383 , 0x01},  //x_odd_inc	
	{0x0387 , 0x01},  //y_odd_inc		

	{0x0344, 0x00 },// X_ADDR_START
	{0x0345, 0x04 },// X_ADDR_START
	{0x0346, 0x00 },// Y_ADDR_START
	{0x0347, 0x04 },// Y_ADDR_START

	{0x0348 , 0x08},/////// addr 
	{0x0349 , 0x03},
	{0x034a , 0x06},
	{0x034b , 0x03},
	
	{0x0401 , 0},    // no scaling
	{0x0405 , 19},  //scaling 19/16


	{0x0340, 0x6},
	{0x0341, 0x10},
	{0x0342, 0x0a},
	{0x0343, 0xac},


	{0x034c , 0x08},//image size
	{0x034d , 0x00},
	{0x034e , 0x06},
	{0x034f , 0x00},

	{0x104 , 0},

	{0x100, 0x01},  //Mode_Select  

#else
	{0x100, 0x00},  //mode seletct
	{0x0103 , 0x01},
{SENSOR_WRITE_DELAY,50},
	{0x100, 0x00},  //mode seletct
	{0x104 , 1},
	{0x301a , 0x18},	
	{0x301b , 0xcc},



	{0x300 , 0x00},
	{0x301 , 0x05}, // 5
	{0x302 , 0x00},
	{0x303 , 0x02}, // 2
	{0x304 , 0x00},
	{0x305 , 0x02},
	{0x306 , 0x00},
	{0x307 , 40},   //20:pclk = mclk ; 40 : pclk =  2* mclk

	//{0x202, 0x04},  //coarse intergration time 


	{0x0383 , 0x01},  //x_odd_inc	
	{0x0387 , 0x01},  //y_odd_inc		

	{0x0348 , 0x08},/////// addr 
	{0x0349 , 0x03},
	{0x034a , 0x06},
	{0x034b , 0x03},
	{0x0401 , 0},    // no scaling
	{0x0405 , 19},  //scaling 19/16
	{0x0340, 0x6},
	{0x0341, 0x10},
	{0x0342, 0x0a},
	{0x0343, 0xac},
	//{0x601 , 0x02},   //test pattern



	{0x034c , 0x08},//image size
	{0x034d , 0x00},
	{0x034e , 0x06},
	{0x034f , 0x00},
	{0x104 , 0},

	{0x100, 0x01},  //Mode_Select 
#endif

};

LOCAL SENSOR_REG_TAB_INFO_T s_MT9T012_resolution_Tab_RAWRGB[]=
{
	// COMMON INIT	
	{ADDR_AND_LEN_OF_ARRAY(MT9T012_RGB_COMMON), 0, 0, 12, 0},
		
	// RAWRGB PREVIEW 1
	{ADDR_AND_LEN_OF_ARRAY(MT9T012_RGB_1024X768), 1024, 768, 12, 0},
	{ADDR_AND_LEN_OF_ARRAY(MT9T012_RGB_2048X1536), 2048, 1536, 12, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0}
};


/************************************************************************************/
/*	sensor color informaion															*/
/************************************************************************************/
/****************************************************************************/
/*****************************capture parameters table***********************/
//gamma table

//because adjust gamma will effect image average luminance, so must adjust taget luminance based on gamma

const LOCAL uint32 s_MT9T012_targetLumiance_gamma[] =
{
40,
50,
60,
70,
75,
80,
85
	
}; 

const LOCAL uint32 s_MT9T012_cap_gamma_level_0_R[ ] =
{

5,16,27,40,56,73,89,112, 132 ,148, 160,185,203,220 ,231,235 
};


// decrease_contrast.m, k=32
const LOCAL uint32 s_MT9T012_cap_gamma_level_1_R[ ] =
{

3,12,22,35,53,68,84,108, 128 ,145, 158,184,202,219 ,231,235

};

// decrease_contrast.m, k=16
const LOCAL uint32 s_MT9T012_cap_gamma_level_2_R[ ] =
{

2,9,18,30,49,65,79,102,124,141,156,183,202,219,231,235
};

// NORMAL CURVE
const LOCAL uint32 s_MT9T012_cap_gamma_level_3_R[ ] =
{


    1,6,14,25,44,60,75,99,120,139,155,183,202,219,231,235
	
 	
};

// increase_contrast.m, k=16
const LOCAL uint32 s_MT9T012_cap_gamma_level_4_R[ ] =
{

1,6,12,21,39,54,67,92,110,129,147,175,198,214,231,235
};

// increase_contrast.m, k=32
const LOCAL uint32 s_MT9T012_cap_gamma_level_5_R[ ] =
{

1, 3, 6, 14, 27,43,59,86,105,123,139,167,189,209,225,235
};

// increase_contrast.m, k=48
const LOCAL uint32 s_MT9T012_cap_gamma_level_6_R[ ] =
{

1,2,4,6,15,27,45,74,101,125,146,175,199,219,233,235


};
const SENSOR_GAMMA_TABLE_T s_MT9T012_cap_gammr_table_R[] = 
{
    {SENSOR_GAMMA_LEVEL_0, s_MT9T012_cap_gamma_level_0_R},
    {SENSOR_GAMMA_LEVEL_1, s_MT9T012_cap_gamma_level_1_R},
    {SENSOR_GAMMA_LEVEL_2, s_MT9T012_cap_gamma_level_2_R},
    {SENSOR_GAMMA_LEVEL_3, s_MT9T012_cap_gamma_level_3_R},
    {SENSOR_GAMMA_LEVEL_4, s_MT9T012_cap_gamma_level_4_R},
    {SENSOR_GAMMA_LEVEL_5, s_MT9T012_cap_gamma_level_5_R},
    {SENSOR_GAMMA_LEVEL_6, s_MT9T012_cap_gamma_level_6_R},
};

const LOCAL uint32 s_MT9T012_cap_gamma_level_0_G[ ] =
{

5,16,27,40,56,73,89,112, 132 ,148, 160,185,203,220 ,231,235 
};


// decrease_contrast.m, k=32
const LOCAL uint32 s_MT9T012_cap_gamma_level_1_G[ ] =
{

3,12,22,35,53,68,84,108, 128 ,145, 158,184,202,219 ,231,235

};

// decrease_contrast.m, k=16
const LOCAL uint32 s_MT9T012_cap_gamma_level_2_G[ ] =
{

2,9,18,30,49,65,79,102,124,141,156,183,202,219,231,235
};

// NORMAL CURVE
const LOCAL uint32 s_MT9T012_cap_gamma_level_3_G[ ] =
{


    1,6,14,25,44,60,75,99,120,139,155,183,202,219,231,235
	
 	
};

// increase_contrast.m, k=16
const LOCAL uint32 s_MT9T012_cap_gamma_level_4_G[ ] =
{

1,6,12,21,39,54,67,92,110,129,147,175,198,214,231,235
};

// increase_contrast.m, k=32
const LOCAL uint32 s_MT9T012_cap_gamma_level_5_G[ ] =
{
1, 3, 6, 14, 27,43,59,86,105,123,139,167,189,209,225,235
};

// increase_contrast.m, k=48
const LOCAL uint32 s_MT9T012_cap_gamma_level_6_G[ ] =
{

1,2,4,6,15,27,45,74,101,125,146,175,199,219,233,235


};
const SENSOR_GAMMA_TABLE_T s_MT9T012_cap_gammr_table_G[] = 
{
    {SENSOR_GAMMA_LEVEL_0, s_MT9T012_cap_gamma_level_0_G},
    {SENSOR_GAMMA_LEVEL_1, s_MT9T012_cap_gamma_level_1_G},
    {SENSOR_GAMMA_LEVEL_2, s_MT9T012_cap_gamma_level_2_G},
    {SENSOR_GAMMA_LEVEL_3, s_MT9T012_cap_gamma_level_3_G},
    {SENSOR_GAMMA_LEVEL_4, s_MT9T012_cap_gamma_level_4_G},
    {SENSOR_GAMMA_LEVEL_5, s_MT9T012_cap_gamma_level_5_G},
    {SENSOR_GAMMA_LEVEL_6, s_MT9T012_cap_gamma_level_6_G},
};

const LOCAL uint32 s_MT9T012_cap_gamma_level_0_B[ ] =
{

5,16,27,40,56,73,89,112, 132 ,148, 160,185,203,220 ,231,235 
};


// decrease_contrast.m, k=32
const LOCAL uint32 s_MT9T012_cap_gamma_level_1_B[ ] =
{

3,12,22,35,53,68,84,108, 128 ,145, 158,184,202,219 ,231,235

};

// decrease_contrast.m, k=16
const LOCAL uint32 s_MT9T012_cap_gamma_level_2_B[ ] =
{

2,9,18,30,49,65,79,102,124,141,156,183,202,219,231,235
};

// NORMAL CURVE
const LOCAL uint32 s_MT9T012_cap_gamma_level_3_B[ ] =
{


    1,6,14,25,44,60,75,99,120,139,155,183,202,219,231,235
	
 	
};

// increase_contrast.m, k=16
const LOCAL uint32 s_MT9T012_cap_gamma_level_4_B[ ] =
{

1,6,12,21,39,54,67,92,110,129,147,175,198,214,231,235
};

// increase_contrast.m, k=32
const LOCAL uint32 s_MT9T012_cap_gamma_level_5_B[ ] =
{

1, 3, 6, 14, 27,43,59,86,105,123,139,167,189,209,225,235
};

// increase_contrast.m, k=48
const LOCAL uint32 s_MT9T012_cap_gamma_level_6_B[ ] =
{

1,2,4,6,15,27,45,74,101,125,146,175,199,219,233,235


};
const SENSOR_GAMMA_TABLE_T s_MT9T012_cap_gammr_table_B[] = 
{
    {SENSOR_GAMMA_LEVEL_0, s_MT9T012_cap_gamma_level_0_B},
    {SENSOR_GAMMA_LEVEL_1, s_MT9T012_cap_gamma_level_1_B},
    {SENSOR_GAMMA_LEVEL_2, s_MT9T012_cap_gamma_level_2_B},
    {SENSOR_GAMMA_LEVEL_3, s_MT9T012_cap_gamma_level_3_B},
    {SENSOR_GAMMA_LEVEL_4, s_MT9T012_cap_gamma_level_4_B},
    {SENSOR_GAMMA_LEVEL_5, s_MT9T012_cap_gamma_level_5_B},
    {SENSOR_GAMMA_LEVEL_6, s_MT9T012_cap_gamma_level_6_B},
};


const LOCAL SENSOR_GAMMA_TABLE_FULL_T s_MT9T012_cap_gamma_table[] =
{
	s_MT9T012_cap_gammr_table_R,
	s_MT9T012_cap_gammr_table_G,
	s_MT9T012_cap_gammr_table_B	
};


//cce table
//k=1.8
const LOCAL int32 s_MT9T012_cap_cce_setting_normal[]=
{

#if 1  //normal saturation
86 , 189 , -18,
-81 , -245 , 326,
295, -324, 29
#elif 0  //no color
77,150,29,
-43,-85,128,
128,-107,-22


#endif
};

const LOCAL int32 s_MT9T012_cap_cce_setting_blackwhite[]=
{
	75, 	169, 	12, 
	0, 		0, 		0, 
	0, 		0, 		0,
};

const LOCAL int32 s_MT9T012_cap_cce_setting_red[]=
{
	113,	85, 	6, 
	-84, 	-55, 	83, 
	249, 	-70,  	-14,
	
};

const LOCAL int32 s_MT9T012_cap_cce_setting_green[]=
{
	38, 	254, 	6, 
	-28, 	-165, 	83, 
	83, 	-209,  	-14,
};

const LOCAL int32 s_MT9T012_cap_cce_setting_blue[]=
{
	38, 	85, 	18, 
	-28, 	-55, 	249, 
	83, 	-70,  	-41,
};

const LOCAL int32 s_MT9T012_cap_cce_setting_yellow[]=
{

	128,	178, 	49, 
	-150, 	-77,  	83,  
	133, 	-97,   	-14,
	
	
 
};

const LOCAL int32 s_MT9T012_cap_cce_setting_sepia[]=
{
	128,	178, 	49, 
	-150, 	-77,  	83,  
	200, 	-97,   	-14,
 
};

const LOCAL int32 s_MT9T012_cap_cce_setting_negative[]=
{
	70, 	201, 	-16, 
	77,  	153,  	-240,
	-240,	192,    38,
};


const LOCAL int32 s_MT9T012_cap_cce_setting_landscape[]=
{
84 ,186 ,-14,
 -55 ,-317 ,372,
 295 ,-334 , 39,
};


const LOCAL int32 s_MT9T012_cap_cce_setting_People[]=
{
84 ,186 ,-14,
 -55 ,-317 ,372,
 295 ,-334 , 39,
};

const LOCAL int32 s_MT9T012_cap_cce_setting_NightMode[]=
{
84 ,186 ,-14,
 -55 ,-317 ,372,
 295 ,-334 , 39,
};

const SENSOR_CCE_SETTING_TABLE_T s_MT9T012_cap_cce_setting_table[] = 
{
    {SENSOR_CCE_NORMAL, 		s_MT9T012_cap_cce_setting_normal},
    {SENSOR_CCE_BLACKWHITE, 	s_MT9T012_cap_cce_setting_blackwhite},
    {SENSOR_CCE_RED, 			s_MT9T012_cap_cce_setting_red},
    {SENSOR_CCE_GREEN, 		s_MT9T012_cap_cce_setting_green},
    {SENSOR_CCE_BLUE, 			s_MT9T012_cap_cce_setting_blue},
    {SENSOR_CCE_YELLOW, 		s_MT9T012_cap_cce_setting_yellow},
    {SENSOR_CCE_SEPIA, 		s_MT9T012_cap_cce_setting_sepia},
    {SENSOR_CCE_NEGATIVE,		s_MT9T012_cap_cce_setting_negative},
    {SENSOR_CCE_LANDSCAPE,   s_MT9T012_cap_cce_setting_landscape },
    {SENSOR_CCE_PEOPLE, s_MT9T012_cap_cce_setting_People },
    {SENSOR_CCE_NIGHTMODE, s_MT9T012_cap_cce_setting_NightMode }

};

//denoise table
const LOCAL int32 s_MT9T012_cap_denoise_nsr[]=
{
	//weight
	128, //NSR_H
	60, //NSR_THR
	20, //NSR_K
	2,	 //NSR_BBV
	
	//limit
	15, //EDGE_LMT
	255, //EDGETHRD
	255, //AVOID_THR 	
	254	 //NSR_WBV
};
/*
SENSOR_CFG_NSR_WEIGHT
*/

// CFA configure table



const LOCAL uint32 s_MT9T012_cap_cfa_2048X1536[] = 
{
	8, //SHARP_THR
	0//SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_1600X1200[] = 
{
	8, //SHARP_THR
	0	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_1280X1024[] = 
{
	8, //SHARP_THR
	0	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_1280X960[] = 
{
	8, //SHARP_THR
	0	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_1024X768[] = 
{
	8, //SHARP_THR
	0	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_800X600[] = 
{
	8, //SHARP_THR
	0	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_640X512[] = 
{
	8, //SHARP_THR
	0	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_640X480[] = 
{
	8, //SHARP_THR
	0	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_352X288[] = 
{
	8, //SHARP_THR
	3	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_320X240[] = 
{
	10, //SHARP_THR
	3	 //SHARP_RATE
};

const LOCAL uint32 s_MT9T012_cap_cfa_160X120[] = 
{
	10, //SHARP_THR
	3	 //SHARP_RATE
};



const LOCAL uint32 s_MT9T012_cap_cfa_reserve[] = 
{
	10,//SHARP_THR
	3	 //SHARP_RATE
};

const SENSOR_CFA_SETTING_T s_MT9T012_cap_cfa[] =
{
	{SENSOR_SET_PARAM_SIZE_2048X1536, s_MT9T012_cap_cfa_2048X1536},
	{SENSOR_SET_PARAM_SIZE_1600X1200, s_MT9T012_cap_cfa_1600X1200},
	{SENSOR_SET_PARAM_SIZE_1280X1024, s_MT9T012_cap_cfa_1280X1024},
	{SENSOR_SET_PARAM_SIZE_1280X960,   s_MT9T012_cap_cfa_1280X960},		//SXGA-
	{SENSOR_SET_PARAM_SIZE_1024X768,   s_MT9T012_cap_cfa_1024X768},
	{SENSOR_SET_PARAM_SIZE_800X600,	   s_MT9T012_cap_cfa_800X600},
	{SENSOR_SET_PARAM_SIZE_640X512,	   s_MT9T012_cap_cfa_640X512},
	{SENSOR_SET_PARAM_SIZE_640X480,	   s_MT9T012_cap_cfa_640X480},
	{SENSOR_SET_PARAM_SIZE_352X288,	   s_MT9T012_cap_cfa_352X288},
	{SENSOR_SET_PARAM_SIZE_320X240,	   s_MT9T012_cap_cfa_320X240},
	{SENSOR_SET_PARAM_SIZE_160X120,	   s_MT9T012_cap_cfa_160X120},
};


const LOCAL uint32 s_MT9T012_Optical_Black[] =
{
	12,
	12,
	12,
	12
};



/****************************************************************************/
/*****************************preview parameters table***********************/
//gamma table
const LOCAL uint32 s_MT9T012_preview_gamma_level_0_R[ ] =
{

5,16,27,40,56,73,89,112, 132 ,148, 160,185,203,220 ,231,235 
};


// decrease_contrast.m, k=32
const LOCAL uint32 s_MT9T012_preview_gamma_level_1_R[ ] =
{

3,12,22,35,53,68,84,108, 128 ,145, 158,184,202,219 ,231,235

};

// decrease_contrast.m, k=16
const LOCAL uint32 s_MT9T012_preview_gamma_level_2_R[ ] =
{

2,9,18,30,49,65,79,102,124,141,156,183,202,219,231,235
};

// NORMAL CURVE
const LOCAL uint32 s_MT9T012_preview_gamma_level_3_R[ ] =
{


    1,6,14,25,44,60,75,99,120,139,155,183,202,219,231,235
	
 	
};

// increase_contrast.m, k=16
const LOCAL uint32 s_MT9T012_preview_gamma_level_4_R[ ] =
{

1,6,12,21,39,54,67,92,110,129,147,175,198,214,231,235
};

// increase_contrast.m, k=32
const LOCAL uint32 s_MT9T012_preview_gamma_level_5_R[ ] =
{

1, 3, 6, 14, 27,43,59,86,105,123,139,167,189,209,225,235
};

// increase_contrast.m, k=48
const LOCAL uint32 s_MT9T012_preview_gamma_level_6_R[ ] =
{

1,2,4,6,15,27,45,74,101,125,146,175,199,219,233,235


};
const SENSOR_GAMMA_TABLE_T s_MT9T012_preview_gammr_table_R[] = 
{
    {SENSOR_GAMMA_LEVEL_0, s_MT9T012_preview_gamma_level_0_R},
    {SENSOR_GAMMA_LEVEL_1, s_MT9T012_preview_gamma_level_1_R},
    {SENSOR_GAMMA_LEVEL_2, s_MT9T012_preview_gamma_level_2_R},
    {SENSOR_GAMMA_LEVEL_3, s_MT9T012_preview_gamma_level_3_R},
    {SENSOR_GAMMA_LEVEL_4, s_MT9T012_preview_gamma_level_4_R},
    {SENSOR_GAMMA_LEVEL_5, s_MT9T012_preview_gamma_level_5_R},
    {SENSOR_GAMMA_LEVEL_6, s_MT9T012_preview_gamma_level_6_R},
};

const LOCAL uint32 s_MT9T012_preview_gamma_level_0_G[ ] =
{

5,16,27,40,56,73,89,112, 132 ,148, 160,185,203,220 ,231,235 
};


// decrease_contrast.m, k=32
const LOCAL uint32 s_MT9T012_preview_gamma_level_1_G[ ] =
{

3,12,22,35,53,68,84,108, 128 ,145, 158,184,202,219 ,231,235

};

// decrease_contrast.m, k=16
const LOCAL uint32 s_MT9T012_preview_gamma_level_2_G[ ] =
{

2,9,18,30,49,65,79,102,124,141,156,183,202,219,231,235
};

// NORMAL CURVE
const LOCAL uint32 s_MT9T012_preview_gamma_level_3_G[ ] =
{


    1,6,14,25,44,60,75,99,120,139,155,183,202,219,231,235
	
 	
};

// increase_contrast.m, k=16
const LOCAL uint32 s_MT9T012_preview_gamma_level_4_G[ ] =
{

1,6,12,21,39,54,67,92,110,129,147,175,198,214,231,235
};

// increase_contrast.m, k=32
const LOCAL uint32 s_MT9T012_preview_gamma_level_5_G[ ] =
{

1, 3, 6, 14, 27,43,59,86,105,123,139,167,189,209,225,235
};

// increase_contrast.m, k=48
const LOCAL uint32 s_MT9T012_preview_gamma_level_6_G[ ] =
{

1,2,4,6,15,27,45,74,101,125,146,175,199,219,233,235


};

const SENSOR_GAMMA_TABLE_T s_MT9T012_preview_gammr_table_G[] = 
{
    {SENSOR_GAMMA_LEVEL_0, s_MT9T012_preview_gamma_level_0_G},
    {SENSOR_GAMMA_LEVEL_1, s_MT9T012_preview_gamma_level_1_G},
    {SENSOR_GAMMA_LEVEL_2, s_MT9T012_preview_gamma_level_2_G},
    {SENSOR_GAMMA_LEVEL_3, s_MT9T012_preview_gamma_level_3_G},
    {SENSOR_GAMMA_LEVEL_4, s_MT9T012_preview_gamma_level_4_G},
    {SENSOR_GAMMA_LEVEL_5, s_MT9T012_preview_gamma_level_5_G},
    {SENSOR_GAMMA_LEVEL_6, s_MT9T012_preview_gamma_level_6_G},
};

const LOCAL uint32 s_MT9T012_preview_gamma_level_0_B[ ] =
{

5,16,27,40,56,73,89,112, 132 ,148, 160,185,203,220 ,231,235 
};


// decrease_contrast.m, k=32
const LOCAL uint32 s_MT9T012_preview_gamma_level_1_B[ ] =
{

3,12,22,35,53,68,84,108, 128 ,145, 158,184,202,219 ,231,235

};

// decrease_contrast.m, k=16
const LOCAL uint32 s_MT9T012_preview_gamma_level_2_B[ ] =
{

2,9,18,30,49,65,79,102,124,141,156,183,202,219,231,235
};

// NORMAL CURVE
const LOCAL uint32 s_MT9T012_preview_gamma_level_3_B[ ] =
{


    1,6,14,25,44,60,75,99,120,139,155,183,202,219,231,235
	
 	
};

// increase_contrast.m, k=16
const LOCAL uint32 s_MT9T012_preview_gamma_level_4_B[ ] =
{

1,6,12,21,39,54,67,92,110,129,147,175,198,214,231,235
};

// increase_contrast.m, k=32
const LOCAL uint32 s_MT9T012_preview_gamma_level_5_B[ ] =
{

1, 3, 6, 14, 27,43,59,86,105,123,139,167,189,209,225,235
};

// increase_contrast.m, k=48
const LOCAL uint32 s_MT9T012_preview_gamma_level_6_B[ ] =
{

1,2,4,6,15,27,45,74,101,125,146,175,199,219,233,235


};
const SENSOR_GAMMA_TABLE_T s_MT9T012_preview_gammr_table_B[] = 
{
    {SENSOR_GAMMA_LEVEL_0, s_MT9T012_preview_gamma_level_0_B},
    {SENSOR_GAMMA_LEVEL_1, s_MT9T012_preview_gamma_level_1_B},
    {SENSOR_GAMMA_LEVEL_2, s_MT9T012_preview_gamma_level_2_B},
    {SENSOR_GAMMA_LEVEL_3, s_MT9T012_preview_gamma_level_3_B},
    {SENSOR_GAMMA_LEVEL_4, s_MT9T012_preview_gamma_level_4_B},
    {SENSOR_GAMMA_LEVEL_5, s_MT9T012_preview_gamma_level_5_B},
    {SENSOR_GAMMA_LEVEL_6, s_MT9T012_preview_gamma_level_6_B},
};



const SENSOR_GAMMA_TABLE_FULL_T s_MT9T012_preview_gamma_table[] =
{
	s_MT9T012_preview_gammr_table_R,
	s_MT9T012_preview_gammr_table_G,
	s_MT9T012_preview_gammr_table_B	
};



//cce table
//k=1.8
const LOCAL int32 s_MT9T012_preview_cce_setting_normal[]=

{

#if 1
86 , 189 , -18,
-81 , -245 , 326,
295, -324, 29
#else
77,150,29,
-43,-85,128,
128,-107,-22
#endif
};


const LOCAL int32 s_MT9T012_preview_cce_setting_blackwhite[]=
{
	75, 	169, 	12, 
	0, 		0, 		0, 
	0, 		0, 		0,
};

const LOCAL int32 s_MT9T012_preview_cce_setting_red[]=
{
	113,	85, 	6, 
	-84, 	-55, 	83, 
	249, 	-70,  	-14,
};

const LOCAL int32 s_MT9T012_preview_cce_setting_green[]=
{
	38, 	254, 	6, 
	-28, 	-165, 	83, 
	83, 	-209,  	-14,
};

const LOCAL int32 s_MT9T012_preview_cce_setting_blue[]=
{
	38, 	85, 	18, 
	-28, 	-55, 	249, 
	83, 	-70,  	-41,
};

const LOCAL int32 s_MT9T012_preview_cce_setting_yellow[]=
{
	128,	178, 	49, 
	-150, 	-77,  	83,  
	133, 	-97,   	-14,
 
};

const LOCAL int32 s_MT9T012_preview_cce_setting_sepia[]=
{
	128,	178, 	49, 
	-150, 	-77,  	83,  
	200, 	-97,   	-14,
 
};

const LOCAL int32 s_MT9T012_preview_cce_setting_negative[]=
{
	75, 	169, 	12, 
	56,  	110,  	-166,
	-166,	139,    27,
};



const LOCAL int32 s_MT9T012_preview_cce_setting_landscape[]=
{
84 ,186 ,-14,
 -55 ,-317 ,372,
 295 ,-334 , 39,
};


const LOCAL int32 s_MT9T012_preview_cce_setting_People[]=
{
84 ,186 ,-14,
 -55 ,-317 ,372,
 295 ,-334 , 39,
};

const LOCAL int32 s_MT9T012_preview_cce_setting_NightMode[]=
{
84 ,186 ,-14,
 -55 ,-317 ,372,
 295 ,-334 , 39,
};

const SENSOR_CCE_SETTING_TABLE_T s_MT9T012_preview_cce_setting_table[] = 
{
    {SENSOR_CCE_NORMAL, 		s_MT9T012_preview_cce_setting_normal},
    {SENSOR_CCE_BLACKWHITE, 	s_MT9T012_preview_cce_setting_blackwhite},
    {SENSOR_CCE_RED, 			s_MT9T012_preview_cce_setting_red},
    {SENSOR_CCE_GREEN, 		s_MT9T012_preview_cce_setting_green},
    {SENSOR_CCE_BLUE, 			s_MT9T012_preview_cce_setting_blue},
    {SENSOR_CCE_YELLOW, 		s_MT9T012_preview_cce_setting_yellow},
    {SENSOR_CCE_SEPIA, 		s_MT9T012_preview_cce_setting_sepia},
    {SENSOR_CCE_NEGATIVE,		s_MT9T012_preview_cce_setting_negative},

    {SENSOR_CCE_LANDSCAPE,   s_MT9T012_preview_cce_setting_landscape },
    {SENSOR_CCE_PEOPLE, s_MT9T012_preview_cce_setting_People },
    {SENSOR_CCE_NIGHTMODE, s_MT9T012_preview_cce_setting_NightMode }


};

//denoise table
const LOCAL int32 s_MT9T012_preview_denoise_nsr[]=
{
	//weight
	128, //NSR_H
	60, //NSR_THR
	20, //NSR_K
	2,	 //NSR_BBV
	
	//limit
	15, //EDGE_LMT
	50, //EDGETHRD
	255, //AVOID_THR 	
	254	 //NSR_WBV
};

// CFA configure table
const LOCAL uint32 s_MT9T012_preview_cfa_conf[] = 
{
	10, //SHARP_THR
	3	 //SHARP_RATE
};

const SENSOR_CFA_SETTING_T s_MT9T012_pre_cfa[] =
{
	{SENSOR_SET_PARAM_SIZE_RESERVE, s_MT9T012_preview_cfa_conf},
	
};

/****************************************************************************/
/*****************************lens correction table***********************/

//NOTE: default values assume corner level is 4/5 of center level. XGFENG 2006/11/16

// NOTE: TEMPERARY DISABLE LNC BY SETTING ALPHA VALUES TO ZERO. XGFENG 2006/11/22

//sensor size 2048X1536 
const LOCAL int32 s_MT9T012_setting_lens_2048X1536[] =
{
	1024, 	//CENTER_X
	768, 	//CENTER_Y
	
	0,//1,  	//ALPHAR
	0,//1,  	//ALPHAG
	0,//1,  	//ALPHAB
	11,  	//ALPHA_SHIFT
	
	1638400  //CENTER_SQR
};

//sensor size 1600X1200
const LOCAL int32 s_MT9T012_setting_lens_1600X1200[] =
{
	800, 	//CENTER_X
	600, 	//CENTER_Y
	
	0,//2,  	//ALPHAR
	0,//2,  	//ALPHAG
	0,//2,  	//ALPHAB
	11,  	//ALPHA_SHIFT
	
	1000000  //CENTER_SQR
};

//sensor size 1280X1024
const LOCAL int32 s_MT9T012_setting_lens_1280X1024[] =
{
	640, 	//CENTER_X
	512,  	//CENTER_Y
	
	0,//2, 		//ALPHAR
	0,//2,  	//ALPHAG
	0,//2, 		//ALPHAB
	10, 		//ALPHA_SHIFT
	
	671744  //CENTER_SQR
};

//sensor size 1280X960
const LOCAL int32 s_MT9T012_setting_lens_1280X960[] =
{
	640, 	//CENTER_X
	480,  	//CENTER_Y
	
	0,//2, 		//ALPHAR
	0,//2,  	//ALPHAG
	0,//2, 		//ALPHAB
	10,  	//ALPHA_SHIFT
	
	640000	//CENTER_SQR
};

//sensor size 1024X768
const LOCAL int32 s_MT9T012_setting_lens_1024X768[] =
{
	512, 	//CENTER_X
	384, 	//CENTER_Y
	
	0,//1,  	//ALPHAR
	0,//1,  	//ALPHAG
	0,//1,  	//ALPHAB
	9,  	//ALPHA_SHIFT
	
	409600  //CENTER_SQR
};

//sensor size 800X600
const LOCAL int32 s_MT9T012_setting_lens_800X600[] =
{
	400, 	//CENTER_X
	300, 	//CENTER_Y
	
	0,//1,  	//ALPHAR
	0,//1,  	//ALPHAG
	0,//1,  	//ALPHAB
	10,  	//ALPHA_SHIFT
	
	730000  //CENTER_SQR
};

//sensor size 640X512
const LOCAL int32 s_MT9T012_setting_lens_640X512[] =
{
	320, 	//CENTER_X
	256, 	//CENTER_Y
	
	0,//1,  	//ALPHAR
	0,//1,  	//ALPHAG
	0,//1,  	//ALPHAB
	8,  	//ALPHA_SHIFT
	
	167936  //CENTER_SQR
};


//sensor size 640X480
const LOCAL int32 s_MT9T012_setting_lens_640X480[] =
{
	320, 	//CENTER_X
	240, 	//CENTER_Y
	
	0,//2,  	//ALPHAR
	0,//2,  	//ALPHAG
	0,//2,  	//ALPHAB
	8,  	//ALPHA_SHIFT
	
	160000  //CENTER_SQR
};

//sensor size 352X288
const LOCAL int32 s_MT9T012_setting_lens_352X288[] =
{
	176, 	//CENTER_X
	144, 	//CENTER_Y
	
	0,//1,  	//ALPHAR
	0,//1,  	//ALPHAG
	0,//1,  	//ALPHAB
	6,  	//ALPHA_SHIFT
	
	51712  //CENTER_SQR
};

//sensor size 320X240
const LOCAL int32 s_MT9T012_setting_lens_320X240[] =
{
	160, 	//CENTER_X
	120, 	//CENTER_Y
	
	0,//1,  	//ALPHAR
	0,//1,  	//ALPHAG
	0,//1,  	//ALPHAB
	6,  	//ALPHA_SHIFT
	
	40000  //CENTER_SQR
};

//sensor size 160X120
const LOCAL int32 s_MT9T012_setting_lens_160X120[] =
{
	80, 	//CENTER_X
	60, 	//CENTER_Y
	
	0,//1,  	//ALPHAR
	0,//1,  	//ALPHAG
	0,//1,  	//ALPHAB
	4,  	//ALPHA_SHIFT
	
	10000  //CENTER_SQR
};

//sensor size reserve
const LOCAL int32 s_MT9T012_setting_lens_reserve[] =
{
	640, 	//CENTER_X
	480,  	//CENTER_Y
	
	0,//2, 		//ALPHAR
	0,//2,  	//ALPHAG
	0,//2, 		//ALPHAB
	10,  	//ALPHA_SHIFT
	
	640000	//CENTER_SQR
};

const SENSOR_LNC_REF_PARAMS_T g_lnc_ref_param = 
{
	320,
	240,
	640,
	480,
	200
};

/*
typedef struct  _SENSOR_len_setting_t
{
	SENSOR_SET_PARAM_SENSOR_SIZE imgsize;
	int32                     *SENSOR_len_setting_ptr; 
}SENSOR_LEN_SETTING_T;
*/
const SENSOR_LEN_SETTING_T s_MT9T012_lens_setting[] = 
{

	{SENSOR_SET_PARAM_SIZE_2048X1536, s_MT9T012_setting_lens_2048X1536},
	{SENSOR_SET_PARAM_SIZE_1600X1200, s_MT9T012_setting_lens_1600X1200},
	{SENSOR_SET_PARAM_SIZE_1280X1024, s_MT9T012_setting_lens_1280X1024},
	{SENSOR_SET_PARAM_SIZE_1280X960,   s_MT9T012_setting_lens_1280X960},		//SXGA-
	{SENSOR_SET_PARAM_SIZE_1024X768,   s_MT9T012_setting_lens_1024X768},
	{SENSOR_SET_PARAM_SIZE_800X600,	   s_MT9T012_setting_lens_800X600},
	{SENSOR_SET_PARAM_SIZE_640X512,	   s_MT9T012_setting_lens_640X512},
	{SENSOR_SET_PARAM_SIZE_640X480,	   s_MT9T012_setting_lens_640X480},
	{SENSOR_SET_PARAM_SIZE_352X288,	   s_MT9T012_setting_lens_352X288},
	{SENSOR_SET_PARAM_SIZE_320X240,	   s_MT9T012_setting_lens_320X240},
	{SENSOR_SET_PARAM_SIZE_160X120,	   s_MT9T012_setting_lens_160X120},

};




LOCAL SENSOR_COLOR_TAB_INFO_T s_MT9T012_color_table =
{
	s_MT9T012_targetLumiance_gamma,
	s_MT9T012_cap_gamma_table,
	s_MT9T012_cap_cce_setting_table,
	s_MT9T012_cap_denoise_nsr,
	s_MT9T012_cap_cfa,
	s_MT9T012_preview_gamma_table,
	s_MT9T012_preview_cce_setting_table,
	s_MT9T012_preview_denoise_nsr,
	s_MT9T012_pre_cfa,
	s_MT9T012_lens_setting,
	s_MT9T012_Optical_Black,
};

/************************************************************************************/
/*	sensor awb informaion															*/
/************************************************************************************/
const LOCAL SENSOR_AWB_LIGHTSOURCE_PARA_T MT9T012_AWB_LIGHTSOURCES[] =
{
	/*Cb,  Cr,Rgain,Ggain,Bgain,thr_ChromatismSum*/

    //here normalize Cb and Cr with Cb = Cb * 256 /Y;
    { -51,  10, 63,  64, 104,    64},    // a light,2856k
    { -44,  -21, 82,  64, 102,    64},    // CWF light,4150k
    { -9,  -38, 100,  64, 76,    64},    // D light,6500k
    { -24,  -60, 93,  64, 103,    64},    //indoor light,4150k
    { 0xff, 0xff,0xff,0xff,0xff,0xff},

     
 
};

/* DSC manual mode gain table for AWB */
const LOCAL SENSOR_AWB_MANUAL_MODE_GAIN_T MT9T012_AWB_MANUAL_MODE_GAIN[] =
{
	/*manual mode,Rgain,Grgain,Gbgain,Bgain*/
	{AWBC_MANUAL_2800K, 63,  64,  62,  104},  
	{AWBC_MANUAL_4000K, 82,  64,  62,  102}, 
	{AWBC_MANUAL_4150K, 82,  64,  62,  100},  
	{AWBC_MANUAL_4400K, 88,  64,  62,  95}, 
	{AWBC_MANUAL_6500K, 100,  64,  62,  76},  
	{AWBC_MANUAL_7000K, 110,  64,  62,  73},
	{AWBC_MANUAL_MAX,	255,  255, 255, 255} 
};


//Complex CTC  window
//D to indoor window


const LOCAL int32 MT9T012_ctc0_window[] = {0 , -26 , -49 , -45};
const LOCAL int32 MT9T012_ctc0_param[] = {1, 0};

//indoor to cwf window
const LOCAL int32 MT9T012_ctc1_window[] = {-26 , -48 , -75 , -69};
const LOCAL int32 MT9T012_ctc1_param[] = {1, 1};

//cwf to A light window
const LOCAL int32 MT9T012_ctc2_window[] = {-48 , -66 , -2406 , -2298};
const LOCAL int32 MT9T012_ctc2_param[] = {18, 40};


const LOCAL SENSOR_AWB_WIN_T MT9T012_awb_advanced_win[] =
{
	{ MT9T012_ctc0_window , MT9T012_ctc0_param},
	{ MT9T012_ctc1_window , MT9T012_ctc1_param},
	{ MT9T012_ctc2_window , MT9T012_ctc2_param},	
};

	
//Simple CTC window


const LOCAL int32 MT9T012_simple_ctc_win[] = {-2,  -65, -75,-65};


const LOCAL uint32 MT9T012_wp_highlight = 220 * 256;
const LOCAL uint32 MT9T012_wp_lowlight = 20 * 256;
	


const LOCAL uint8 MT9T012_awb_max_gain = 0x80;
const LOCAL uint8 MT9T012_awb_min_gain = 0x40;

const LOCAL uint8 MT9T012_s_awb_initialRgain = 93;
const LOCAL uint8 MT9T012_s_awb_initialBgain = 103;


LOCAL SENSOR_AWB_INFO_T s_MT9T012_AWB_info = 
{	
	&MT9T012_awb_max_gain,
	&MT9T012_awb_min_gain,
	&MT9T012_s_awb_initialRgain,
	&MT9T012_s_awb_initialBgain,
	
	&MT9T012_wp_highlight,
	&MT9T012_wp_lowlight,
	
	MT9T012_simple_ctc_win,
		
	MT9T012_AWB_LIGHTSOURCES,
	MT9T012_AWB_MANUAL_MODE_GAIN,
	MT9T012_awb_advanced_win,
};

/************************************************************************************/
/*	sensor ae informaion															*/
/************************************************************************************/
//sdy@add new AE algorithm bafsed on  camera.
//This algorithm from DSC algorhtinm
//AES



const LOCAL uint32 MT9T012_ae_target_luminance =70;
const LOCAL int32 MT9T012_ae_max_50hz_index = 230;
const LOCAL int32 MT9T012_ae_max_night_index = 249;
const LOCAL int32 MT9T012_ae_50hz_initial_index = 100;
const LOCAL int32 MT9T012_ae_night_initial_index = 150;
const LOCAL int32 MT9T012_ae_wdead_zone = 8;


const LOCAL uint8 MT9T012_AE_WEIGHTS_Centre_Weights[]={	
									1,1,1,1,1,1,1,1,
									1,2,2,2,2,2,2,1,
									1,2,4,4,4,4,2,1,
									1,2,4,4,4,4,2,1,
									1,2,4,4,4,4,2,1,
									1,2,4,4,4,4,2,1,
									1,2,2,2,2,2,2,1,
									1,1,1,1,1,1,1,1	};		
									
const LOCAL uint8 MT9T012_AE_WEIGHTS_Average[]={	
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1,
									1,1,1,1,1,1,1,1	};		


const LOCAL uint8 MT9T012_AE_WEIGHTS_Centre[]={   ///for backup light
									0,0,0,0,0,0,0,0,
									0,0,0,0,0,0,0,0,
									0,0,1,1,1,1,0,0,
									0,0,1,1,1,1,0,0,
									0,0,1,1,1,1,0,0,
									0,0,1,1,1,1,0,0,
									0,0,1,1,1,1,0,0,
									0,0,0,0,0,0,0,0	};	

const LOCAL SENSOR_AE_WEIDHTS_T MT9T012_ae_weights[] = 
{
	{SENSOR_AE_MEA_AEVERAGE , MT9T012_AE_WEIGHTS_Average },
	{SENSOR_AE_MEA_CENTRE_WEIGHT , MT9T012_AE_WEIGHTS_Centre_Weights},
	{SENSOR_AE_MEA_CENTRE, MT9T012_AE_WEIGHTS_Centre}
};



//60HZ
const LOCAL uint16 MT9T012_AES[250]={
	50830,50636,47323,44227,41334,38630,36103,33741,31534,29471,/*0*/
	27543,25741,24057,22483,21012,19637,18352,17151,16029,14980,
	14000,13084,12228,11428,10680,9981,9328,8718,8148,7615,

	7117,6651,6216,5809,5429,5074,4742,4432,4142,3871,/*3*/
	3618,3381,3160,2953,2760,2579,2410,2252,2105,1967,
	1838,1718,1606,1501,1403,1311,1225,1145,1070,1000,

	1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,/*6*/
	1000,1000,1000,1000,1000,500, 500, 500, 500, 500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500,

	500, 250, 250, 250, 250, 250, 250, 250, 250, 250,/*9*/
	250, 250, 250, 250, 250, 250, 250, 200, 200, 200, /*119*/
	
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 	 
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*129*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /*149*/
	 
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*159*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*189*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*219*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*249*/
}; 


const LOCAL uint16 MT9T012_AEG[250]={
        
	/*30fps 0-69*/
	0,1,2,3,4,0,1,2,3,0,
	1,2,0,1,2,0,1,2,0,1,
	0,1,0,1,0,1,0,1,0,1,
	
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,

	
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	0x0a,0x0b,0x0c,0x0d,0x0e,0x00,0x00,0x01,0x02,0x03,
	0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d, /*89 */

	0x0e,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08, /*99*/
	0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x0a,0x0b,0x0c, 	
	0x0d,0x0e,0x0f,	0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	
	0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,
	0x2b,0x2c,0x2d,0x2e,0x2f,0x30,	0x31,0x32,0x33,0x34,/*149*/

	0x36,0x38,0x3a,0x3c,0x3e,
	0x40,0x42,0x44,0x46,0x48,
	0x50,0x52,0x54,0x56,0x58,
	0x5a,0x5c,0x5e,0x60,0x62,
	0x64,0x66,0x68,0x6a,0x6c,
	0x6e,0x70,0x72,0x74,0x76,

	0x78,0x7a,0x7c,0x7e,0x80,
	0x82,0x84,0x86,0x88,0x8a,  
	0x8c,0x8e,0x90,0x92,0x94,
	0x96,0x98,0x9a,0x9c,0x9e,
	0xa0,0xa2,0xa4,0xa6,0xa8,
	0xaa,0xac,0xae,0xb0,0xb2,

	0xb4,0xb6,0xb8,0xba,0xbc,
	0xbe,0xc0,0xc2,0xc4,0xc6,
	0xc8,0xca,0xcc,0xce,0xd0,
	0xd2,0xd4,0xd6,0xd8,0xda,
	
	0xdc,0xde,0xe0,0xe2,0xe4,
	0xe6,0xe8,0xea,0xec,0xee,
	0xef, 0xf0, 0xf1,0xf2, 0xf3,
	0xf4, 0xf5, 0xf6,0xf7, 0xf8,
	
	
	
	

};

//60HZ
const LOCAL uint16 MT9T012_AES_60HZ[250]={
	50830,50636,47323,44227,41334,38630,36103,33741,31534,29471,/*0*/
	27543,25741,24057,22483,21012,19637,18352,17151,16029,14980,
	14000,13084,12228,11428,10680,9981,9328,8718,8148,7615,

	7117,6651,6216,5809,5429,5074,4742,4432,4142,3871,/*3*/
	3618,3381,3160,2953,2760,2579,2410,2252,2105,1967,
	1838,1718,1606,1501,1403,1311,1225,1200,1200,1200,

	1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,/*6*/
	1200,1200,1200,1200,1200, 600, 600, 600, 600, 600,
	600, 600, 600, 600, 600, 600, 600, 600, 600, 600,

	600, 300, 300, 300, 300, 300, 300, 300, 300, 300,/*9*/
	300, 300, 300, 300, 300, 300, 300, 200, 200, 200, /*109*/
	
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 	 
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*129*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, /*149*/
	 
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*159*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*189*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*219*/
}; 


const LOCAL uint16 MT9T012_AEG_60HZ[250]={
        


		/*30fps 0-69*/
	0,1,2,3,4,0,1,2,3,0,
	1,2,0,1,2,0,1,2,0,1,
	0,1,0,1,0,1,0,1,0,1,
	
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,

	/*30fps 70-122*/
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	0x0a,0x0b,0x0c,0x0d,0x0e,0x00,0x00,0x01,0x02,0x03,
	0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d, /*89 */

	0x0e,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08, /*99*/
	0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x05,0x06,0x07, /*109*/

	0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,
	0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,
	0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,
	0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,
	0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,/*169*/
	0x50,0x51,0x52,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,
	0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,
	0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
	0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,/*219*/


};


//50HZ night mode
const LOCAL  uint16 MT9T012_AES_NIGHT[250]={


	//50830,50636,47323,44227,41334,38630,36103,33741,31534,29471,/*0*/
	//27543,25741,24057,22483,21012,19637,18352,17151,16029,14980,
	//14000,13084,12228,11428,10680,9981,9328,8718,8148,7615,

	7117,6651,6216,5809,5429,5074,4742,4432,4142,3871,/*1*/
	3618,3381,3160,2953,2760,2579,2410,2252,2105,1967,
	1838,1718,1606,1501,1403,1311,1225,1145,1070,1000,

	1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,/*3*/
	1000,1000,1000,1000,1000,  500, 500,  500,  500,  500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500,

	500, 250, 250, 250, 250, 250, 250, 250, 250, 250,/*6*/
	250, 250, 250, 250, 250, 250, 250, 200, 200, 200, 	
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 
	
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*9*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,	
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,/*12*/
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,

	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,/*15*/
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,

	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,/*18*/
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,

	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  /*21*/
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
	100, 100, 100, 100, 100, 100, 100, 100, 100, 100,  
}; 


const LOCAL  uint16 MT9T012_AEG_NIGHT[250]={
        
	/*30fps 0-69*/
	/*
	0,1,2,3,4,0,1,2,3,0,
	1,2,0,1,2,0,1,2,0,1,
	0,1,0,1,0,1,0,1,0,1,
	*/
	
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,

	/*30fps 70-122*/
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	0x0a,0x0b,0x0c,0x0d,0x0e,0x00,0x00,0x01,0x02,0x03,
	0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,
	
	0x0e,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x0a,0x0b,0x0c, 	
	0x0d,0x0e,0x0f,	0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	
	0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
	0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,	0x30,	
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
	
	0x1a,0x1b,0x1c,0x1d,0x1e, 0x1f ,0x20,0x21,0x22,0x23,
	0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,
	0x2e,0x2f,	0x30,0x31,0x32,0x33, 0x34,0x35,0x36,0x37,
	
	0x38,0x39,0x3a,0x3b,0x3c,0x3d, 0x3e,0x3f, 0x40,0x41,
	0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,
	0x4c,0x4d,0x4e,0x4f,0x50,0x51,	0x52,0x53,0x54,0x55,
	
	0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f, 
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
	0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70, 0x71,0x72, 0x73,
	
	0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,
	0x7e,0x7f, 0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
	0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,
	0x92,0x93, 0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,
	

};

const LOCAL  uint16 MT9T012_AES_NIGHT_60HZ[250]={
	50830,50636,47323,44227,41334,38630,36103,33741,31534,29471,/*0*/
	27543,25741,24057,22483,21012,19637,18352,17151,16029,14980,
	14000,13084,12228,11428,10680,9981,9328,8718,8148,7615,

	7117,6651,6216,5809,5429,5074,4742,4432,4142,3871,/*3*/
	3618,3381,3160,2953,2760,2579,2410,2252,2105,1967,
	1838,1718,1606,1501,1403,1311,1225,1145,1070,1000,

	1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,/*6*/
	1000,1000,1000,1000,1000,  500, 500,  500,  500,  500,
	500, 500, 500, 500, 500, 500, 500, 500, 500, 500,

	500, 250, 250, 250, 250, 250, 250, 250, 250, 250,/*9*/
	250, 250, 250, 250, 250, 250, 250, 200, 200, 200, /*119*/
	
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 	 
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,/*129*/
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120, /*149*/
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120,/*159*/
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
	120, 120, 120, 120, 120, 120, 120, 120, 120, 120,
	
}; 


const LOCAL  uint16 MT9T012_AEG_NIGHT_60HZ[250]={
        
	/*30fps 0-69*/
	0,1,2,3,4,0,1,2,3,0,
	1,2,0,1,2,0,1,2,0,1,
	0,1,0,1,0,1,0,1,0,1,
	
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,

	/*30fps 70-122*/
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	0x0a,0x0b,0x0c,0x0d,0x0e,0x00,0x00,0x01,0x02,0x03,
	0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d, /*89 */

	0x0e,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08, /*99*/
	0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x0a,0x0b,0x0c, /*109*/
	
	0x0d,0x0e,0x0f,	0x10,0x11,0x12,0x13,0x14,0x15,0x16,
	0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,
	0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,	
	
       0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e, 0x1f,
       0x20,0x21,0x22,0x23, 0x24,0x25,0x26,0x27,0x28,0x29,
       0x2a,0x2b,0x2c,0x2d, 0x2e,0x2f,0x30,0x31,0x32,0x33, 
       0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,
       0x3e,0x3f, 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
       0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,
       0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,
       0x5c,0x5d,0x5e,0x5f,	0x60,0x61,0x62,0x63,0x64,0x65,
       

};

const LOCAL SENSOR_AE_table_T MT9T012_ae_table[] = 
{
	{MT9T012_AES , MT9T012_AEG}, //normal 50HZ
	{MT9T012_AES_60HZ, MT9T012_AEG_60HZ},//normal 60HZ
	{MT9T012_AES_NIGHT, MT9T012_AEG_NIGHT}, //night 60hz
	{MT9T012_AES_NIGHT_60HZ, MT9T012_AEG_NIGHT_60HZ},
};


LOCAL SENSOR_AE_INFO_T s_MT9T012_ae_info =
{
	
    &MT9T012_ae_target_luminance ,
    &MT9T012_ae_max_50hz_index ,
    &MT9T012_ae_max_night_index ,
    &MT9T012_ae_50hz_initial_index ,
    &MT9T012_ae_night_initial_index ,
    &MT9T012_ae_wdead_zone,
        
	MT9T012_ae_weights,
	MT9T012_ae_table,
};


LOCAL SENSOR_IOCTL_FUNC_TAB_T s_MT9T012_ioctl_func_tab = 
{
// Internal 
	PNULL,
	PNULL,
	PNULL,
	MT9T012_Identify,
	
	PNULL,//MT9T012_WriteReg,			// write register
	PNULL,//MT9T012_ReadReg,			// read  register	
	PNULL,
	PNULL,

// External
	set_ae_enable,
	set_hmirror_enable,
	set_vmirror_enable,
	
	PNULL,
	PNULL,
	PNULL,
	PNULL,
	
	set_preview_mode,	
	set_image_effect,

	MT9T012_BeforeSnapshot,
	MT9T012_AfterSnapshot,

	PNULL,
	
	read_ev_value,
	write_ev_value,
	read_gain_value,
	write_gain_value,
	read_gain_scale,
	set_frame_rate,
	PNULL,
	PNULL,
	PNULL,
	PNULL,	

	PNULL,
	PNULL,
	PNULL,
	
};

#if 1
LOCAL SENSOR_RAW_INFO_T s_MT9T1012_rawinfo = 
{
	&s_MT9T012_color_table,
	&s_MT9T012_AWB_info,
	&s_MT9T012_ae_info
};

PUBLIC SENSOR_INFO_T g_MT9T012_raw_info =
{
	MT9T012_I2C_ADDR_W,				// salve i2c write address
	MT9T012_I2C_ADDR_R, 			// salve i2c read address
	
	SENSOR_I2C_VAL_8BIT|\
	SENSOR_I2C_REG_16BIT,			// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_N|\
	SENSOR_HW_SIGNAL_VSYNC_P|\
	SENSOR_HW_SIGNAL_HSYNC_P,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved											
											
	// preview mode
	SENSOR_ENVIROMENT_NORMAL|\
	SENSOR_ENVIROMENT_NIGHT|\
	SENSOR_ENVIROMENT_SUNNY,		
	
	// image effect
	0,
	
	// while balance mode
	0,
		
	0,								// bit[0:7]: count of step in brightness, contrast, sharpness, saturation
									// bit[8:31] reseved
	
	SENSOR_LOW_PULSE_RESET,			// reset pulse level
	20,								// reset pulse width(ms)
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	1,								// count of identify code
	0x3000, 0x16,					// supply two code to identify sensor.
	0x3001, 0x0,					// for Example: index = 0-> Device id, index = 1 -> version id											
											
	SENSOR_AVDD_2800MV,				// voltage of avdd	
	
	2048,							// max width of source image
	1536,							// max height of source image
	"MT9T012",						// name of sensor												

	SENSOR_IMAGE_FORMAT_RAW,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_RAWRGB_GR,// pattern of input image form sensor;			

	s_MT9T012_resolution_Tab_RAWRGB,	// point to resolution table information structure
	&s_MT9T012_ioctl_func_tab,		// point to ioctl function table
			
	&s_MT9T1012_rawinfo,			// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	
	
};
#else

const PUBLIC SENSOR_INFO_T g_MT9T012_raw_info =
{
	MT9T012_I2C_ADDR_W,
	MT9T012_I2C_ADDR_R,	
	
	0,							// count of step in brightness, 1ntrast, sharpness, saturation
	0,							// 0:negative; 1:positive -> polarily of pixel clock	
	1,							// 0:negative; 1:positive -> polarily of vertical synchronization signal
	1,							// 0:negative; 1:positive -> polarily of horizontal synchronization signal

	0,							// image effect don't need to set while output raw

	SENSOR_LOW_PULSE_RESET,		// reset pulse level
	100,							// reset pulse width(ms)
	
	1,	
	0x3000, 0x16,					// product id
	0x3001, 0x00,

	SENSOR_AVDD_2800MV,			// avdd voltage

	SENSOR_HIGH_LEVEL_PWDN,		// power down level
	
		
	2048,
	1536,
	"MT9T012",					

	SENSOR_IMAGE_FORMAT_RAW,	// image format
	SENSOR_IMAGE_PATTERN_RAWRGB_GR,
	
	s_MT9T012_resolution_Tab_RAWRGB,
	&s_MT9T012_ioctl_func_tab,

	SENSOR_I2C_VAL_8BIT|SESNOR_I2C_REG_16BIT,

	&s_MT9T012_color_table,
	&s_MT9T012_AWB_info,
	&s_MT9T012_ae_info,	
};

#endif
//MT9T012 register address is 16bit.
LOCAL uint32 MT9T012_ReadReg(uint32 subaddr)
{
    uint8  cmd[4] = {0};    
	uint16 ret_val = 0;	
    uint8 addr_w = MT9T012_I2C_ADDR_W;
	uint8 addr_r = MT9T012_I2C_ADDR_R;


	cmd[0] = (uint8)((subaddr>>8)&0xff );
	cmd[1] = (uint8)( subaddr&0xff );

	cmd[2] = (uint8)((subaddr>>16)&0xff );
	cmd[3] = (uint8)((subaddr>>24)&0xff );
	
	I2C_WriteCmdArr(addr_w, cmd,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd[0], 1, SCI_TRUE);


	MT9T012_TRACE_DEBUG("test I2C 1: %d, 2: %d , 3: %d, 4:%d" , cmd[1] , cmd[0] ,cmd[2] ,cmd[3]);
	MT9T012_TRACE_DEBUG("test I2C subaddr = 0x%x" ,subaddr);
	ret_val = (uint32)cmd[0];

	return  ret_val;

}


#if 0
LOCAL uint32 MT9T012_WriteReg(uint32 param)
{
    uint8 cmd[4] = {0};
	uint8 addr_w = MT9T012_I2C_ADDR_W;

    cmd[1] = (uint8)((param>>16)&0xff);
	cmd[2] = (uint8)(param&0xff);
	cmd[0] = (uint8)((param>>24)&0xff);


	MT9T012_TRACE_DEBUG("write register : %d ,%d , %d  ", cmd[0] , cmd[1] , cmd[2] );

	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);


	return SENSOR_OP_SUCCESS;
}
#endif


LOCAL uint32 set_ae_enable(uint32 enable)
{
   // unsigned char value;

	MT9T012_TRACE_DEBUG("set_ae_enable: enable = %d", enable);

	return 0;
}

LOCAL uint32 set_hmirror_enable(uint32 enable)
{
   // unsigned char value;

	MT9T012_TRACE_DEBUG("set_hmirror_enable: enable = %d", enable);

	return 0;
}

LOCAL uint32 set_vmirror_enable(uint32 enable)
{
    //unsigned char value;

	MT9T012_TRACE_DEBUG("set_vmirror_enable: enable = %d", enable);

	return 0;
}


//read exposure time;
//care only coarse exposure time; reg : 0x202 ; 0x203
//do not care fine exposure time
LOCAL uint32 read_ev_value(uint32 value)
{
	uint8 expo_low = 0;
	uint8 expo_high = 0;
	uint32 read_exposure = 0;

	uint8 cmd[2] = {0};

    uint8 addr_w = MT9T012_I2C_ADDR_W;
	uint8 addr_r = MT9T012_I2C_ADDR_R;


	cmd[0] = 0x02 ;  //read exposure low bytes
	cmd[1] = 0x03;
	I2C_WriteCmdArr(addr_w, cmd,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd[0], 1, SCI_TRUE);

	expo_low = cmd[0];

	cmd[0] = 0x02 ;  //read exposure hight bytes
	cmd[1] = 0x02;

	I2C_WriteCmdArr(addr_w, cmd,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd[0], 1, SCI_TRUE);

	expo_high = cmd[0];

	read_exposure = (uint32)expo_low |( (uint32)expo_high <<8 );

	return read_exposure;
}


//the max exposure lines is 0xffff
//write exposure time;
//care only coarse exposure time; reg : 0x202 ; 0x203
//do not care fine exposure time
LOCAL uint32 write_ev_value(uint32 exposure_value)
{
	uint8 expo_low;
	uint8 expo_high;

	uint16 frame_length = MT9T012_PREVIEW_FRAMELENGTH;
	uint16 Minframe_length = MT9T012_PREVIEW_FRAMELENGTH;
	uint32 pclk = MT9T012_PREVIEW_PCLK;
	uint32 line_length = MT9T012_PREVIEW_LINELENGTH;
	//uint32 frame_length = 0x310;

	uint32 expTime;


	uint8 cmd[3] = {0};

	uint8 addr_w = MT9T012_I2C_ADDR_W;
	uint8 addr_r = MT9T012_I2C_ADDR_R;

	
    expTime = pclk /line_length * 10/exposure_value;
	
	
	if(0 == expTime)
	{
	      expTime = 1;  //avoid black under very very light environment; such as sun
	}

	

	expo_low = (uint8)(expTime&0xff);
	expo_high = (uint8)( (expTime>>8)&0xff );
	
	MT9T012_TRACE_DEBUG("write_ae_value: %x", expTime);

//{0x0340, 0x01},
//{0x0341, 0xf4},

	
       cmd[0] = 0x03;  //read exposure low bytes
	cmd[1] = 0x40;
	I2C_WriteCmdArr(addr_w, cmd,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd[0], 1, SCI_TRUE);

	frame_length = (uint16)cmd[0] <<8;  //high bytes

	cmd[0] = 0x03 ;  //read exposure hight bytes
	cmd[1] = 0x41;

	I2C_WriteCmdArr(addr_w, cmd,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd[0], 1, SCI_TRUE);

	frame_length = frame_length |(uint16)cmd[0];

	cmd[0] = 0x01 ; //parameter hold
	cmd[1] = 0x04;
	cmd[2] = 0x01;		
	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

	
	if( (expTime ) >Minframe_length ) //frame length must large than exposure time
	{

		cmd[0] = 0x03 ;  //write frame length low bytes
		cmd[1] = 0x41;
		cmd[2] = (uint8)expo_low;		
		I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

		cmd[0] = 0x03 ;  //write frame length  hight bytes
		cmd[1] = 0x40;
		cmd[2] = (uint8)expo_high;		
		I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);
		
	} 

	else 
	{
		
		cmd[0] = 0x03 ;  //write frame length low bytes
		cmd[1] = 0x41;
		cmd[2] = (uint8)(Minframe_length &0xff);		
		I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

		cmd[0] = 0x03 ;  //write frame length  hight bytes
		cmd[1] = 0x40;
		cmd[2] = (uint8)((Minframe_length >> 8)&0xff);		
		I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);
	
	}

	


	cmd[0] = 0x02 ;  //write ev low bytes
	cmd[1] = 0x03;
	cmd[2] = (uint8)expo_low;		
	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

	cmd[0] = 0x02 ;  //write ev hight bytes
	cmd[1] = 0x02;
	cmd[2] = (uint8)expo_high;		
	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);
	
	cmd[0] = 0x01;
	cmd[1] = 0x04;
	cmd[2] = 0x00;		
	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

	
	return 0;	
}


//read analogue gain
//do not care digital gain
LOCAL uint32 read_gain_value(uint32 value)
{
	uint16 analogue_gain = 0;
    uint8  analogue_gain_low = 0;
    uint32 read_gain = 1;
	uint8  cmd[2] = {0};    
	
       uint8 addr_w = MT9T012_I2C_ADDR_W;
	uint8 addr_r = MT9T012_I2C_ADDR_R;


	
	cmd[0] = (uint8)(0x30);  //analogue gain low bytes
	cmd[1] = (uint8)(0x5f);

	I2C_WriteCmdArr(addr_w, cmd,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd[0], 1, SCI_TRUE);

	analogue_gain_low = cmd[0];

	analogue_gain = (uint16)analogue_gain_low;   // do not care digital gain


	if ( 0x80 == (analogue_gain&0x80))
	{
		read_gain= 2;
	}

	read_gain = read_gain * (analogue_gain & 0x7f);
	
	
	MT9T012_TRACE_DEBUG("read_gain_value: %x", read_gain);
	
	return read_gain;
}


/*gain vlaue must be the scale * gain*/
//do not care digital gain
//gain_value can not exceed 16*16 = 256;

LOCAL uint32 write_gain_value(uint32 gain_value)
{
	//uint8 write_gain00 = 0;
	uint8 addr_w = MT9T012_I2C_ADDR_W;
	uint16 half_analogue_gain = 0;
    uint8  analogue_gain_low = 0x00;
   // uint8  analogue_gain_high = 0x00;
	uint8 cmd[3] = {0};//parameter hold

	MT9T012_TRACE_DEBUG("write_gain_value: %x", gain_value);

	gain_value = gain_value + 16;
	if(gain_value > 16)
	{
		half_analogue_gain = (uint8)( (gain_value+1)/2 ); //avoid bit loss

		half_analogue_gain =  half_analogue_gain | 0x80;

		analogue_gain_low =  half_analogue_gain;
		
	}

	else
	{
	     analogue_gain_low = gain_value + 1;  //to avoid gain == 0;
	}
       
	
	cmd[0] = 0x01 ;
	cmd[1] = 0x04;
	cmd[2] = 0x01;		
	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

	cmd[0] = 0x30 ;  //write gain
	cmd[1] = 0x5f;
	cmd[2] = (uint8)analogue_gain_low;		
	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

	
	cmd[0] = 0x01;
	cmd[1] = 0x04;
	cmd[2] = 0x00;		
	I2C_WriteCmdArr(addr_w, cmd, 3, SCI_TRUE);

	return 0;
}

LOCAL uint32 read_gain_scale(uint32 value)
{
	return SENSOR_GAIN_SCALE;
}

LOCAL uint32 set_preview_mode(uint32 preview_mode)
{
	MT9T012_TRACE_DEBUG("set_preview_mode: preview_mode = %d", preview_mode);
		
	return 0;
}

#if 0
LOCAL uint32 set_brightness(uint32 level)
{
	//uint16 i;

	MT9T012_TRACE_DEBUG("set_brightness: level = %d", level);
	
	return 0;
}

LOCAL uint32 set_contrast(uint32 level)
{
	//uint16 i;

	MT9T012_TRACE_DEBUG("set_contrast: level = %d", level);
	
	return 0;
}

LOCAL uint32 set_sharpness(uint32 level)
{
	//uint16 i;

	MT9T012_TRACE_DEBUG("set_sharpness: level = %d", level);
	
	return 0;
}

LOCAL uint32 set_saturation(uint32 level)
{
	//uint16 i;

	MT9T012_TRACE_DEBUG("set_saturation: level = %d", level);
	
	return 0;
}
#endif
LOCAL uint32 set_image_effect(uint32 effect_type)
{
	//uint16 i;

	MT9T012_TRACE_DEBUG("-----------set_image_effect: effect_type = %d------------", effect_type);
	
	return 0;
}

LOCAL uint32 set_frame_rate(uint32 param)
{
	//uint32 clkrc = 0;
	
	MT9T012_TRACE_DEBUG("set_frame_rate: param = %d", param);
	
	
		

	return 0;
}

LOCAL uint32 MT9T012_Identify(uint32 param)
{
	#define MT9T012_PID_VALUE	0x16	
	#define MT9T012_PID_ADDR		0x3000
	
	//uint8 cmd[3] = {0};
	//uint8 addr_w = MT9T012_I2C_ADDR_W;
	uint32 pid_value = 0;
	BOOLEAN ret_value = 0XFF;
	uint16 i2c_cmd = 0;



	i2c_cmd = MT9T012_PID_ADDR;
       pid_value = MT9T012_ReadReg(i2c_cmd );
	MT9T012_TRACE_DEBUG("SDY: This sensr is MT9T012 = %d" , pid_value );


	if(MT9T012_PID_VALUE == pid_value)
	{
		MT9T012_TRACE_DEBUG("This sensr is MT9T012_");
		ret_value = 0;
	}
	else
	{
		MT9T012_TRACE_DEBUG("This sensr is not MT9T012_");
	       ret_value = 0xff;
	}

	
	return (uint32)ret_value;
}



LOCAL void Sensor_Set_ChangeMega_MT9T012(uint32 AEParam) 
{
    uint8 addr_w = MT9T012_I2C_ADDR_W;
	uint8 addr_r = MT9T012_I2C_ADDR_R;
	uint8 cmd_setting[3] = {0};
	uint8 temp_high;
	uint8 temp_low;
	//uint16 Pre_Frame_Length = 0X310;//MT9T012_PREVIEW_FRAMELENGTH;
	uint16 Cap_Frame_Length = 0x610;
	uint16 Pre_Line_Length = MT9T012_PREVIEW_LINELENGTH;
	uint16 Cap_Line_Length = MT9T012_CAPTURE_LINELENGTH;

	uint32 Pre_PCLK =  MT9T012_PREVIEW_PCLK;  //MHz
	uint32 Cap_PCLK = MT9T012_CAPTURE_PCLK;  //MHz


	uint16 infact_Gain = 1;
	uint16 Multi_Gain = 1;
	uint8 Pre_Gain;
	uint16 Pre_Exposure_line;

	uint8 Cap_Gain;
	uint16 Cap_Exposure_line;
	uint16 Cap_Fine;
	uint32 uTemp;
	

	//read preview frame length

#if 1
	//read analogue gain
	cmd_setting[0] = 0x30;
	cmd_setting[1] = 0x5f;

	I2C_WriteCmdArr(addr_w, cmd_setting,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd_setting[0], 1, SCI_TRUE);

	Pre_Gain = cmd_setting[0];

	//read preview exposure time line
	cmd_setting[0] = 0x02;
	cmd_setting[1] = 0x02;

	I2C_WriteCmdArr(addr_w, cmd_setting,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd_setting[0], 1, SCI_TRUE);

	temp_high = cmd_setting[0];

	
	cmd_setting[0] = 0x02;
	cmd_setting[1] = 0x03;

	I2C_WriteCmdArr(addr_w, cmd_setting,  2,  SCI_TRUE);
	I2C_ReadCmdArr(addr_r, &cmd_setting[0], 1, SCI_TRUE);


	temp_low = cmd_setting[0];
	Pre_Exposure_line = ( ( (uint16)temp_high<<8 ) & 0xff00) | ( (uint16)temp_low & 0xff );


       MT9T012_TRACE_DEBUG("SDY:   MT9T012_BeforeSnapshot -1: Pre_Gain = %d ; preview Exp: %d", Pre_Gain ,Pre_Exposure_line);

#else
 	
	Pre_Exposure_line =  s_ae_last_ev ;
	Pre_Exposure_line = Pre_PCLK * 10/ Pre_Exposure_line /  Pre_Line_Length;
  	Pre_Gain = s_ae_last_gain ;
#endif

       if( (Pre_Gain & 0x80) == 0x80)
       {
       	Pre_Gain = (Pre_Gain & 0x7f) * 2;
	
       }
	s_GainReg =Pre_Gain;
	s_ExpReg_high =(uint8)((Pre_Exposure_line >>8)&0xff);
       s_ExpReg_low = (uint8)(Pre_Exposure_line &0xff);
	MT9T012_TRACE_DEBUG("SDY:   MT9T012_BeforeSnapshot: Pre_Gain = %d ; preview Exp: %d", Pre_Gain ,Pre_Exposure_line);

#ifdef CARE_CAPTURE_FRAMERATE	//care about capture frame rate

	if( (Pre_PCLK/Pre_Exposure_line/ Pre_Line_Length / 2)  > (Cap_PCLK/Cap_Frame_Length / Cap_Line_Length) )
	{
		Cap_Gain = 	( (Pre_Gain&0x7f)>>1) |(0x80 & Pre_Gain);		  // 1/2 preview gain  , but keep the frame rate
		uTemp = Cap_PCLK *10 /Pre_PCLK  * Pre_Exposure_line * Pre_Line_Length /10;

		Cap_Exposure_line = uTemp * 2 / Cap_Line_Length ;	

		Cap_Fine = uTemp * 2%Cap_Line_Length;
		
		
	}
	else
	{

		Cap_Gain =  Pre_Gain;		  
		uTemp = Cap_PCLK  /Pre_PCLK * Pre_Exposure_line * Pre_Line_Length ;
		Cap_Exposure_line = (uint16)(uTemp / Cap_Line_Length);	
		
		Cap_Fine = uTemp %Cap_Line_Length;
	}
#else

		//infact_Gain = ((Pre_Gain>>7) +1 ) * (Pre_Gain&0x7f);  
		infact_Gain = Pre_Gain;
		if(infact_Gain > 128)//
		{
			Multi_Gain = 8;
		}
		else if(infact_Gain > 64)
		{
			Multi_Gain = 4;
		}
		else if(infact_Gain > 32)
		{
			Multi_Gain = 2;
		}
		else if(infact_Gain > 16)
		{
			Multi_Gain = 1;		
		}
		else
		{
			Multi_Gain = 1;		
		
		}
			
			
		//Cap_Gain = (uint8)	(( infact_Gain/Multi_Gain/2) |0x80) ;		  // 1/2 preview gain  , but keep the frame rate

		Cap_Gain = 	infact_Gain / Multi_Gain;
		
		uTemp = Cap_PCLK *10 /Pre_PCLK  * Pre_Exposure_line * Pre_Line_Length /10;

		Cap_Exposure_line = uTemp  * Multi_Gain / Cap_Line_Length ;	

		Cap_Fine = uTemp * Multi_Gain%Cap_Line_Length;

		if((Cap_Exposure_line) > Cap_Frame_Length)
		{
			cmd_setting[0] = 0x03 ;  //write frame length low bytes
			cmd_setting[1] = 0x41;
			cmd_setting[2] = (uint8)((Cap_Exposure_line +2) &0xff);		
			I2C_WriteCmdArr(addr_w, cmd_setting, 3, SCI_TRUE);

			cmd_setting[0] = 0x03 ;  //write frame length  hight bytes
			cmd_setting[1] = 0x40;
			cmd_setting[2] = (uint8)( ( (Cap_Exposure_line +2) >>8 )&0xff);		
			I2C_WriteCmdArr(addr_w, cmd_setting, 3, SCI_TRUE);
		}		

#endif

    	MT9T012_TRACE_DEBUG("SDY:   MT9T012_BeforeSnapshot: infact_Gain = %d, Multi_Gain = %d Cap_Gain = %d ; Cap Exp: %d", infact_Gain, Multi_Gain,Cap_Gain ,Cap_Exposure_line);
	MT9T012_TRACE_DEBUG("SDY:   MT9T012_BeforeSnapshot: Cap_Line_Length = %d, Pre_Line_Length = %d", Cap_Line_Length, Pre_Line_Length);

	cmd_setting[0] = 0x01;  //parameter hold
	cmd_setting[1] = 0x04;
	cmd_setting[2] = 0x01;

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	
	cmd_setting[0] = 0x30;  //set gain
	cmd_setting[1] = 0x5f;
	cmd_setting[2] = Cap_Gain;

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	
	
	cmd_setting[0] = 0x02;  // set coarse 
	cmd_setting[1] = 0x02;
	cmd_setting[2] = (uint8) ((Cap_Exposure_line >> 8) & 0xff); 

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);

		
	cmd_setting[0] = 0x02;
	cmd_setting[1] = 0x03;
	cmd_setting[2] = (uint8)(Cap_Exposure_line & 0xff);

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	/*
	cmd_setting[0] = 0x02;  //set fine
	cmd_setting[1] = 0x00;
	cmd_setting[2] = (uint8) ((Cap_Fine >> 8) & 0xff); 

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);

		
	cmd_setting[0] = 0x02;
	cmd_setting[1] = 0x01;
	cmd_setting[2] = (uint8)(Cap_Fine & 0xff);

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	*/
	cmd_setting[0] = 0x01;  //enable parameter
	cmd_setting[1] = 0x04;
	cmd_setting[2] = 0x00;

	
	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	

	

	
	

	
}

LOCAL uint32 MT9T012_BeforeSnapshot(uint32 param)
{
	SENSOR_MODE_E sensor_mode = (SENSOR_MODE_E)param;
	
	MT9T012_TRACE_DEBUG("SDY:   MT9T012_BeforeSnapshot: param = %d", param);
		
	if(SENSOR_MODE_SNAPSHOT_ONE_FIRST == sensor_mode)
	{
		Sensor_Set_ChangeMega_MT9T012(param);
		
	}

//	SCI_Sleep(5000);
	
	return 0;
	
}

	

LOCAL void Sensor_Set_ChangeXGA_MT9T012(uint32 AEParam) 
{


    uint8 addr_w = MT9T012_I2C_ADDR_W;
	///uint8 addr_r = MT9T012_I2C_ADDR_R;
	uint8 cmd_setting[3] ;




	cmd_setting[0] = 0x01;  //parameter hold
	cmd_setting[1] = 0x04;
	cmd_setting[2] = 0x01;

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	
	cmd_setting[0] = 0x30;  //set gain
	cmd_setting[1] = 0x5f;
	cmd_setting[2] = s_GainReg;

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	
	
	cmd_setting[0] = 0x02;  // set coarse 
	cmd_setting[1] = 0x02;
	cmd_setting[2] = s_ExpReg_high; 

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);

		
	cmd_setting[0] = 0x02;
	cmd_setting[1] = 0x03;
	cmd_setting[2] = s_ExpReg_low;

	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	

	
	cmd_setting[0] = 0x01;  //enable parameter
	cmd_setting[1] = 0x04;
	cmd_setting[2] = 0x00;

	
	I2C_WriteCmdArr(addr_w, cmd_setting,  3,  SCI_TRUE);
	
	
}
LOCAL uint32 MT9T012_AfterSnapshot(uint32 param)
{
	SENSOR_MODE_E sensor_mode = (SENSOR_MODE_E)param;
	
	MT9T012_TRACE_DEBUG("SDY:   MT9T012_BeforeSnapshot: param = %d", param);
		
	if(SENSOR_MODE_SNAPSHOT_ONE_FIRST == sensor_mode)
	{
		Sensor_Set_ChangeXGA_MT9T012(param);
		
	}

//	SCI_Sleep(5000);
	
	return 0;
	
}
