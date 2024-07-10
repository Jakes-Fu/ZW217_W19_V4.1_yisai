/******************************************************************************
 ** File Name:      siv120b.c                                         *
 ** Author:        	cloud6                                             	  *
 ** Date:           26/05/2009                                            *
 ** Copyright:      XUNRUI. All Rights Reserved.        *
 ** Description:    implementation of digital camera register interface       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **                                  *
 ******************************************************************************/

#ifndef _siv120b_C_
#define _siv120b_C_

#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define siv120b_I2C_ADDR_W    			0x66
#define siv120b_I2C_ADDR_R    			0x67
#define siv120b_I2C_ACK				0x0

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
#define    siv120b_DELAY_MS(ms)            {\
                                              uint32 end = SCI_GetTickCount() + ms;\
                                              while (SCI_GetTickCount() < end)\
                                              ;\
                                          }

#if 0
LOCAL BOOLEAN g_is_1M = FALSE;
uint8 Temp_WbCapture;
LOCAL BOOLEAN Flag_Wb = 1; //normal:1 bool
static uint32 g_iPreviewExp = 0;
PUBLIC void  siv120b_Set_Is_1M(BOOLEAN is_one_mega)
{
    g_is_1M  = is_one_mega;
}

PUBLIC BOOLEAN  siv120b_Get_Is_1M()
{
    return g_is_1M ;
   //return FALSE;
}
#endif

/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
// OV give the parameters

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/


__align(4) const SENSOR_REG_T siv120b_YVU_640X480[] =
{
	{0x00,0x00},
	{0x04,0x00},
	{0x05,0x07}, //VGA Output
	{0x10,0x13},
	{0x11,0x25},
	{0x12,0x21},
	{0x13,0x17}, //ABS
	{0x16,0xCF},
	{0x17,0xAA}, //Internal LDO On
	#if 1	
	{0x20,0x00},
	{0x21,0x6E},
	{0x23,0xA1},
	{0x00,0x01},
	{0x34,0x84},
	#else
	{0x20,0x00},
	{0x21,0x6E},
	{0x23,0x1D},
	{0x00,0x01},
	{0x34,0x42},
	#endif
	
	{0x00,0x00},
	{0x40,0x00},
	{0x41,0x00},
	{0x42,0x00},
	{0x43,0x00},
	{0x00,0x01},
	{0x11,0x0D},  //0x0a max step
	{0x12,0x84}, // D65 target
	{0x13,0x84}, // D30 target
	{0x14,0x80}, // D20 target
	{0x16,0x01},
	{0x19,0x8E},
	{0x1A,0x06},
	{0x1B,0x88},
	{0x1D,0x02}, //Initial shutter time
	{0x1E,0x28}, //Initial analog gain
	{0x40,0x38},
	{0x41,0x20},
	{0x42,0x20},
	{0x43,0x08},
	{0x44,0x08},
	{0x45,0x08},
	{0x46,0x17},
	{0x47,0x1D},
	{0x48,0x1E},
	{0x49,0x1F},
	{0x4A,0x1F},
	{0x4B,0x1F},
	{0x4C,0x1F},
	{0x4D,0x1F},
	{0x4E,0x1A},
	{0x4F,0x14},
	{0x50,0x11},
	{0x51,0x0F},
	{0x52,0x0D},
	{0x53,0x0C},
	{0x54,0x0A},
	{0x55,0x09},
	{0x60,0xFF},
	{0x61,0xFF},
	{0x62,0xFF},
	{0x63,0xFF},
	{0x64,0xFF},
	{0x65,0xFF},
	{0x66,0x00},
	{0x67,0x50},
	{0x68,0x50},
	{0x69,0x50},
	{0x6A,0x50},
	{0x6B,0x00},
	{0x6C,0x06},
	{0x70,0xD4}, //Anti Saturation On
	{0x77,0xc0},
	{0x78,0xd0},
	{0x79,0x74},
	{0x90,0xC8},
	{0x00,0x02},
	{0x10,0xD0},
	{0x11,0xC0},
	{0x12,0x80},
	{0x13,0x80},
	{0x14,0x7e},
	{0x15,0xFE},
	{0x16,0x90},
	{0x17,0xEA},
	{0x18,0x80},
	{0x19,0x8a},
	{0x1A,0x73},
	{0x1B,0x95},
	{0x1C,0x68},
	{0x1D,0xA0},
	{0x1E,0x70},
	{0x20,0xE8},
	{0x21,0x60},
	{0x22,0xA4},
	{0x23,0x20},
	{0x24,0xFF},
	{0x25,0x20},
	{0x26,0x0F},
	{0x27,0x10},
	{0x28,0x1A},
	{0x29,0xB8},
	{0x2A,0x90},
	{0x30,0x00},
	{0x31,0x10},
	{0x32,0x00},
	{0x33,0x10},
	{0x34,0x02},
	{0x35,0x76},
	{0x36,0x01},
	{0x37,0xD6},
	{0x40,0x01},
	{0x41,0x04},
	{0x42,0x08},
	{0x43,0x10},
	{0x44,0x12},
	{0x45,0x35},
	{0x46,0x64},
	{0x50,0x33},
	{0x51,0x20},
	{0x52,0xE5},
	{0x53,0xFB},
	{0x54,0x13},
	{0x55,0x26},
	{0x56,0x07},
	{0x57,0xF5},
	{0x58,0xEA},
	{0x59,0x21},
	{0x62,0x7E},
	{0x63,0x9D}, //D30 to D20 for R
	{0x64,0xBE}, //D30 to D20 for B
	{0x65,0x9D}, //D20 to D30 for R
	{0x66,0xBE}, //D20 to D30 for B
	{0x67,0xC2}, //D65 to D30 for R
	{0x68,0x9C}, //D65 to D30 for B
	{0x69,0xC2}, //D30 to D65 for R
	{0x6A,0x9C}, //D30 to D65 for B
	{0x00,0x03},
	{0x10,0xFF}, // IDP function enable
	{0x11,0x1D}, // PCLK polarity
	{0x12,0xFD}, // Y,Cb,Cr order sequence
	{0x8C,0x18}, // Color matrix select at dark condition
	{0x17,0xB8},
	{0x18,0x18},
	{0x19,0x48},
	{0x1A,0x48},
	{0x1B,0x3F},
	{0x1C,0x00},
	{0x1D,0x48},
	{0x1E,0x48},
	{0x1F,0x4F},
	{0x20,0x06}, // Normal illumiinfo start
	{0x21,0x0F}, // Dark illumiinfo start
	//GAMMA
	{0x30,0x00},
	{0x31,0x08},
	{0x32,0x10},
	{0x33,0x20},
	{0x34,0x40},
	{0x35,0x5C},
	{0x36,0x72},
	{0x37,0x86},
	{0x38,0x96},
	{0x39,0xa4},
	{0x3A,0xb2},
	{0x3B,0xc9},
	{0x3C,0xDD},
	{0x3D,0xEF},
	{0x3E,0xF8},
	{0x3F,0xFF},
	
	{0x40,0x11},
	{0x41,0x55},
	{0x42,0x55},
	{0x43,0x43},
	{0x44,0x21},
	{0x45,0x00},
	{0x46,0x22},
	{0x47,0x00},
	{0x48,0x11},
	{0x49,0x00},
	{0x4A,0x11},
	{0x4B,0x00},
	{0x4C,0x01},
	{0x4D,0x00},
	{0x4E,0x04},
	{0x4F,0x44},
	{0x50,0xF6},
	{0x51,0x84},
	{0x52,0x00},
	{0x53,0x00},
	{0x54,0x00},
	{0x55,0x00},
	{0x56,0x10},
	{0x57,0xDA},
	{0x58,0xFF},
	{0x60,0x2F},
	{0x61,0xB7},
	{0x62,0xB7},
	{0x63,0xB7},
	{0x64,0x0C},
	{0x65,0x10},
	{0x66,0x08},
	{0x67,0x10},
	{0x68,0x00},
	{0x69,0x00},
	{0x6A,0x00},
	{0x6B,0x00},
	{0x6C,0xA0},
	{0x6D,0x10},
	{0x6E,0x08},
	{0x6F,0x18},
	{0x71,0x3B},
	{0x72,0xC1},
	{0x73,0x04},
	{0x74,0x0F},
	{0x75,0x1E},
	{0x76,0x12},
	{0x77,0xF3},
	{0x78,0xCF},
	{0x79,0x3E},
	{0x7A,0x3D},
	{0x7B,0xC4},
	{0x7C,0xFF},
	{0x7D,0x10},
	{0x7E,0x1D},
	{0x7F,0x13},
	{0x80,0xF2},
	{0x81,0xC4},
	{0x82,0x4A},
	{0x83,0x3C},
	{0x84,0xC2},
	{0x85,0x01},
	{0x86,0x10},
	{0x87,0x1F},
	{0x88,0x0F},
	{0x89,0xF6},
	{0x8A,0xD2},
	{0x8B,0x39},
	{0x90,0x28},   //0x20 EDGE
	{0x91,0x48},   //0x38
	{0x92,0x04},
	{0x93,0x12},
	{0x94,0x06},
	{0x95,0x10},
	{0x96,0x04},
	{0x97,0x12},
	{0x98,0xFF},
	{0x99,0x20},
	{0x9A,0x20},
	{0x9B,0x28},
	{0x9C,0x1A},
	{0x9D,0x10},
	{0x9E,0x00},
	{0x9F,0x10},
	{0xA0,0x10},
	{0xA1,0x02},
	{0xA2,0x02},
	{0xA3,0x04},
	{0xA4,0x20},
	{0xA5,0x1A},
	{0xA6,0x10},
	{0xA7,0x00},
	
	{0xA8,0x10},
	{0xA9,0x12},
	{0xAA,0x12},  //0x14
	{0xAB,0x00},  //0x08
	{0xAC,0x14},  //0x10 Contrast
	
	{0xB0,0xFF}, //Y Top
	{0xB1,0x00}, //Y Bottom
	{0xB2,0xFF},
	{0xB3,0x00},
	{0xB4,0xFF},
	{0xB5,0x00},
	{0xB9,0x1A}, //Ilimininfo Start
	{0xBA,0x38}, //Slope
	{0xC0,0x24},
	{0xC1,0x00},
	{0xC2,0x80},
	{0xC3,0x00},
	{0xC4,0xE0},
	{0xDD,0x4F}, // ENHCTRL
	{0xDE,0xBA}, // NOIZCTRL
	{0xDF,0x28},
	{0xE0,0x70},
	{0xE1,0x90},
	{0xE2,0x08},
	{0xE3,0x10},
	{0xE4,0x40},
	{0xE5,0x15},
	{0xE6,0x28},
	{0xE7,0x04},
	{0x00,0x01},
	{0x10,0x80},
	{0x00,0x00},
	{0x03,0xC5},
	{0xff, 0xff},
};

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL siv120b_WriteReg( uint8  subaddr, uint8 data )
{

	#ifndef	_USE_DSP_I2C_
		//uint8 cmd[2];
		//cmd[0]	=	subaddr;
		//cmd[1]	=	data;

		//I2C_WriteCmdArr(siv120b_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg(subaddr, data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW:"SENSOR: siv120b_WriteReg reg/value(%x,%x) !!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_385_112_2_18_0_32_37_1356,(uint8*)"dd", subaddr, data);

}

LOCAL uint8 siv120b_ReadReg( uint8  subaddr)
{
	uint8 value = 0;

	#ifndef	_USE_DSP_I2C_
		//I2C_WriteCmdArr(siv120b_I2C_ADDR_W, &subaddr, 1, SCI_TRUE);
		//I2C_ReadCmd(siv120b_I2C_ADDR_R, &value, SCI_TRUE);
		value=Sensor_ReadReg(subaddr);
	#else
		value = (uint16)DSENSOR_IICRead((uint16)subaddr);
	#endif

	return value;
}

/******************************************************************************/
// Description: sensor probe function
// Author:     benny.zou
// Input:      none
// Output:     result
// Return:     0           successful
//             others      failed
// Note:       this function only to check whether sensor is work, not identify
//              whitch it is!!
/******************************************************************************/
LOCAL uint32 siv120b_identify(uint32 param)//(void)
{
    uint8 reg[2]    = {0x01, 0x02};
    uint8 value[2]  = {0x12, 0x11};
    uint8 ret       = 0;
    uint32 i;
    uint8 err_cnt = 0;
    uint32 nLoop = 1000;

//SCI_TRACE_LOW:"siv120b_Identify"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_421_112_2_18_0_32_37_1357,(uint8*)"" );



    for(i = 0; i<2; )
    {
        nLoop = 1000;

         Sensor_WriteReg(0x00,0x00);
		 
        ret = siv120b_ReadReg(reg[i]);

		//SCI_TRACE_LOW:"siv120b read ret is %X"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_433_112_2_18_0_32_37_1358,(uint8*)"d",ret );

        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
            	//SCI_TRACE_LOW:"siv120b Fail"
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_440_112_2_18_0_32_37_1359,(uint8*)"" );
                return 0xFF;
            }
            else
            {
                //Masked by frank.yang,SCI_Sleep() will cause a  Assert when called in boot precedure
                //SCI_Sleep(10);
                while(nLoop--);
                continue;
            }
        }
        err_cnt = 0;
        i++;
    }

     //SCI_TRACE_LOW:"siv120b succ"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_455_112_2_18_0_32_37_1360,(uint8*)"" );
//    s_sensor_ID = SENSOR_siv120b;
    return SCI_SUCCESS;
}


LOCAL uint32 siv120b_hmirror_enable(uint32 param)
{
	  Sensor_WriteReg(0x00,0x00);
    Sensor_WriteReg(0x04,0x01);
	 return SCI_SUCCESS;
}

LOCAL uint32 siv120b_vmirror_enable(uint32 param)
{
	 return SCI_SUCCESS;
}


__align(4) const SENSOR_REG_T siv120b_brightness_tab[][3]=
{

  	{
    	     {0x00, 0x03},
            {0xab, 0xa8},
            {0xFF, 0xFF},
    },

    {
     		{0x00, 0x03},
            {0xab, 0x98},
            {0xFF, 0xFF},
    },

    {
    		{0x00, 0x03},
            {0xab, 0x88},
            {0xFF, 0xFF},
    },

    {
    		{0x00, 0x03},
            {0xab, 0x08},
            {0xFF, 0xFF},
    },

    {
   		{0x00, 0x03},
            {0xab, 0x18},
            {0xFF, 0xFF},
    },
    {
   		{0x00, 0x03},
            {0xab, 0x28},
            {0xFF, 0xFF},
    },
    {
   		{0x00, 0x03},
            {0xab, 0x38},
            {0xFF, 0xFF},
    },

};
LOCAL uint32 siv120b_set_brightness(uint32 level)
{
    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv120b_brightness_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
//   SCI_SLEEP(1000);
    //SCI_TRACE_LOW:"set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_531_112_2_18_0_32_37_1361,(uint8*)"d", level);

    return 0;
}




__align(4) const SENSOR_REG_T siv120b_contrast_tab[][3]=
{

	{
    		{0x00, 0x03},
            {0xa8, 0x07},
            {0xFF, 0xFF},
    },

    {
     		{0x00, 0x03},
            {0xa8, 0x0a},
            {0xFF, 0xFF},
    },

    {
    		{0x00, 0x03},
            {0xa8, 0x0d},
            {0xFF, 0xFF},
    },

    {
    		{0x00, 0x03},
            {0xa8, 0x10},
            {0xFF, 0xFF},
    },

    {
   		{0x00, 0x03},
            {0xa8, 0x13},
            {0xFF, 0xFF},
    },
    {
   		{0x00, 0x03},
            {0xa8, 0x16},
            {0xFF, 0xFF},
    },
    {
   		{0x00, 0x03},
            {0xa8, 0x19},
            {0xFF, 0xFF},
    },
};

LOCAL uint32 siv120b_set_contrast(uint32 level)
{
    uint16 i;

    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv120b_contrast_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_595_112_2_18_0_32_37_1362,(uint8*)"d", level);

    return 0;
}

__align(4) const SENSOR_REG_T siv120b_ev_tab[][7]=
{


    {
		{0x00,0x01},
		{0x12,0x50},
		{0x13,0x50},
		{0x14,0x4d},
		{0x70,0xd4},
		{0x79,0x60},
		{0xff,0xff},
	},

	{
		{0x00,0x01},
		{0x12,0x60},
		{0x13,0x60},
		{0x14,0x5d},
		{0x70,0xd4},
		{0x79,0x68},
		{0xff,0xff},
	},

	{
		{0x00,0x01},
		{0x12,0x74},
		{0x13,0x74},
		{0x14,0x70},
		{0x70,0xd4},
		{0x79,0x70},
		{0xff,0xff},
	},

	{//default ,low light
		{0x00,0x01},
		{0x12,0x84},
		{0x13,0x84},
		{0x14,0x80},
		{0x70,0x00},
		{0x79,0x74},
		{0xff,0xff},
	},

	{
		{0x00,0x01},
		{0x12,0x94},
		{0x13,0x94},
		{0x14,0x90},
		{0x70,0x00},
		{0xff,0xff},
		{0xff,0xff},
	},

	{
		{0x00,0x01},
		{0x12,0xa4},
		{0x13,0xa4},
		{0x14,0xa0},
		{0x70,0x00},
		{0xff,0xff},
		{0xff,0xff},
	},

	{
		{0x00,0x01},
		{0x12,0xb4},
		{0x13,0xb4},
		{0x14,0xb0},
		{0x70,0x00},
		{0xff,0xff},
		{0xff,0xff},
	},

};
LOCAL uint32 siv120b_set_ev(uint32 level)
{
    uint16 i;


    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv120b_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

     if( (level+1) > sizeof(siv120b_ev_tab)/(2*sizeof(SENSOR_REG_T)) )
    {
    	   SCI_ASSERT(0);
    }

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_ev: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_693_112_2_18_0_32_38_1363,(uint8*)"d", level);

    return 0;
}



LOCAL uint32 siv120b_set_anti_flicker(uint32 level)
{

if(level)  //50Hz
         {
        Sensor_WriteReg(0x00, 0x00);
        Sensor_WriteReg(0x20, 0x10);
        Sensor_WriteReg(0x21, 0xc3);
        Sensor_WriteReg(0x23, 0x3d);
        Sensor_WriteReg(0x00, 0x01);
        Sensor_WriteReg(0x34, 0x50);

         }
else   //60Hz
	 {
	 Sensor_WriteReg(0x00, 0x00);
        Sensor_WriteReg(0x20, 0x10);
        Sensor_WriteReg(0x21, 0xc3);
        Sensor_WriteReg(0x23, 0x4d);
        Sensor_WriteReg(0x00, 0x01);
        Sensor_WriteReg(0x34, 0x60);
	 


		
        }

    return 0;
}




LOCAL uint32 siv120b_set_awb(uint32 mode)
{
	DCAMERA_PARAM_WB_MODE_E awb_mode = (DCAMERA_PARAM_WB_MODE_E)mode;

	switch(awb_mode)
	{
		case DCAMERA_WB_MODE_INCANDESCENCE:

			 //SCI_TRACE_LOW:"siv120b: DCAMERA_WB_MODE_INCANDESCENCE"
			 SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_741_112_2_18_0_32_38_1364,(uint8*)"");
                      Sensor_WriteReg(0x00, 0x02);
			 Sensor_WriteReg(0x10, 0x00);
			 Sensor_WriteReg(0x60, 0x98);
 			 Sensor_WriteReg(0x61, 0xc8);
		break;

		case DCAMERA_WB_MODE_FLUORESCENT:

			 //SCI_TRACE_LOW:"siv120b: DCAMERA_WB_MODE_FLUORESCENT"
			 SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_750_112_2_18_0_32_38_1365,(uint8*)"");
                      Sensor_WriteReg(0x00, 0x02);
			 Sensor_WriteReg(0x10, 0x00);
			 Sensor_WriteReg(0x60, 0xaa);
 			 Sensor_WriteReg(0x61, 0xbe);
		break;

		case DCAMERA_WB_MODE_SUN:

			//SCI_TRACE_LOW:"siv120b: DCAMERA_WB_MODE_SUN"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_759_112_2_18_0_32_38_1366,(uint8*)"");
                      Sensor_WriteReg(0x00, 0x02);
			 Sensor_WriteReg(0x10, 0x00);
			 Sensor_WriteReg(0x60, 0xc2);
 			 Sensor_WriteReg(0x61, 0x9e);

		break;

		case DCAMERA_WB_MODE_CLOUD:

			//SCI_TRACE_LOW:"siv120b: DCAMERA_WB_MODE_CLOUD"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_769_112_2_18_0_32_38_1367,(uint8*)"");
		        Sensor_WriteReg(0x00, 0x02);
			 Sensor_WriteReg(0x10, 0x90);
			 Sensor_WriteReg(0x60, 0xd0);
 			 Sensor_WriteReg(0x61, 0x88);

		break;

              case  DCAMERA_WB_MODE_AUTO:


		       //SCI_TRACE_LOW:"siv120b: DCAMERA_WB_MODE_AUTO"
		       SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_780_112_2_18_0_32_38_1368,(uint8*)"");
                      Sensor_WriteReg(0x00, 0x02);
			 Sensor_WriteReg(0x10, 0xD3);


		break;

		default:

		        SCI_ASSERT(0);

		break;

	}
}

LOCAL uint32 Sensor_Write_Regs( SENSOR_REG_T* sensor_reg_ptr )
{
    uint32 i;

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    return 0;
}



LOCAL uint32 siv120b_set_video_mode(uint32 video_mode)
{

    if(video_mode)// video
    {

	 Sensor_WriteReg(0x00, 0x00);
        Sensor_WriteReg(0x04, 0x04);
        Sensor_WriteReg(0x00, 0x03);
        Sensor_WriteReg(0xab, 0x15);
    }
    else // nomal
    {
        
    }

    SCI_Sleep(100);

    	
    return 0;
}


LOCAL uint32 siv120b_set_preview_mode(uint32 preview_mode)
{
    if(preview_mode)
	{
		/*
		siv120b_WriteReg(0x00, 0x00);
		siv120b_WriteReg(0x03, 0x02);
		siv120b_WriteReg(0x00, 0x01);
		siv120b_WriteReg(0x33, 0x0E);
		siv120b_WriteReg(0x00, 0x00);
		siv120b_WriteReg(0x03, 0xC5);
		*/
              siv120b_WriteReg(0x00, 0x01);
		siv120b_WriteReg(0x11, 0x14);
		siv120b_WriteReg(0x00, 0x03);
		siv120b_WriteReg(0xab, 0x18);
	}
	else
	{//Night Mode
		/*
		siv120b_WriteReg(0x00, 0x00);
		siv120b_WriteReg(0x03, 0x02);
		siv120b_WriteReg(0x00, 0x01);
		siv120b_WriteReg(0x33, 0x14);
		siv120b_WriteReg(0x00, 0x00);
		siv120b_WriteReg(0x03, 0xC5);
		*/
              siv120b_WriteReg(0x00, 0x01);
		siv120b_WriteReg(0x11, 0x0E);  //0x0A
		siv120b_WriteReg(0x00, 0x03);
		siv120b_WriteReg(0xab, 0x84);
		
	}
	SCI_Sleep(100);
return 0;
}




__align(4) const SENSOR_REG_T siv120b_image_effect_tab[][5]=
{
	 //const uint8 siv120b_SENSOR_SEPIA_NORMAL [][3] =
	{
		{0x00,0x03},
		{0xB6,0x00},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_SEPIA_BLACKWHITE [][3] =
	{
		{0x00,0x03},
		{0xB6,0x40},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff},		
	},

	//const uint8 siv120b_SENSOR_SEPIA_RED [][3] =
	{
		{0x00,0x03},
		{0xB6,0x80},
		{0xB7,0x60},
		{0xB8,0xC0},
		{0xff,0xff},

	},


	//const uint8 siv120b_SENSOR_SEPIA_GREEN [][3] =
	{
		{0x00,0x03},
		{0xB6,0x80},
		{0xB7,0x50},
		{0xB8,0x50},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_SEPIA_BLUE [][3] =
	{

	       {0x00,0x03},
		{0xB6,0x80},
		{0xB7,0xC0},
		{0xB8,0x60},
		{0xff,0xff},
	},


	//const uint8 siv120b_SENSOR_SEPIA_YELLOW [][3] =
	{

		{0x00,0x03},
		{0xB6,0x80},
		{0xB7,0x14},
		{0xB8,0xA0},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_SEPIA_NEGATIVE [][3] =
	{

		{0x00,0x03},
		{0xB6,0x20},
		{0xB7,0xC0},
		{0xB8,0x60},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_COLOR_INV [][3] =
	{
	       {0x00,0x03},
		{0xB6,0x80},
		{0xB7,0x60},
		{0xB8,0xA0},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_GRAY_INV [][3] =
	{
		{0x00,0x03},
		{0xB6,0x20},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_EMBOSSMENT [][3] =
	{
		{0x00,0x03},
		{0xB6,0x08},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_SKETCH [][3] =
	{
		{0x00,0x03},
		{0xB6,0x04},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff},
	},

	//const uint8 siv120b_SENSOR_SEPIA_MAGENTA [][3] =
	{
		{0x00,0x03},
		{0xB6,0x80},
		{0xB7,0x00},
		{0xB8,0xA0},
		{0xff,0xff},
	},

	//const uint8 siv120b_VFLIP_ENABLE[][3] =
	{
		{0x00,0x00},
		{0x04,0x02},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff},
	},

	//const uint8 siv120b_VFLIP_DISABLE[][3] =
	{
		{0x00,0x00},
		{0x04,0x00},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff},
    },

};

LOCAL uint32 siv120b_set_image_effect(uint32 effect_type)
{
    uint16 i;


    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv120b_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"-----------set_image_effect: effect_type = %d------------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_1022_112_2_18_0_32_38_1369,(uint8*)"d", effect_type);

    return 0;
}

LOCAL uint32 siv120b_ae_aeb_enable(uint32 ae_enable, uint32 awb_enable)
{
	uint16 ae_value = 0;
	uint16 awb_value = 0;
	
		siv120b_WriteReg(0x00, 0x01);
	if(0x01==ae_enable)
	{
		siv120b_WriteReg(0x00, 0x01);
	ae_value = siv120b_ReadReg(0x10);
		ae_value|=0x80;
	}
	else if(0x00==ae_enable)
	{
		siv120b_WriteReg(0x00, 0x01);
	ae_value = siv120b_ReadReg(0x10);
		ae_value&=0x00;
	}
	//	siv120b_WriteReg(0x00, 0x01);
	//siv120b_WriteReg(0x10, ae_value);




	siv120b_WriteReg(0x00, 0x02);
	if(0x01==awb_enable)
	{
	siv120b_WriteReg(0x00, 0x02);
	awb_value = siv120b_ReadReg(0x10);

           awb_value|=0x80;
	}
	else if(0x00==awb_enable)
	{
	siv120b_WriteReg(0x00, 0x02);
	awb_value = siv120b_ReadReg(0x10);

         ae_value&=0x00;
	}
	//	siv120b_WriteReg(0x00, 0x02);
	//siv120b_WriteReg(0x10, awb_value);


	return 0;
}

LOCAL uint32 siv120b_before_snapshot(uint32 para)
{
#if 0
	uint8 reg_val = 0;
	uint16 exposal_time=0x00;
       uint32 iExp;
//SCI_TRACE_LOW:"siv120b_before_snapshot"
SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_1079_112_2_18_0_32_39_1370,(uint8*)"");

	siv120b_ae_aeb_enable(0x00, 0x00);
	
	

		if(TRUE == siv120b_Get_Is_1M())
			{
			siv120b_WriteReg(0x00, 0x00);
	              siv120b_WriteReg(0x03, 0x02);
	            //  siv120b_WriteReg(0x04, 0x0C);
				  siv120b_WriteReg(0x00, 0x00);
	exposal_time = siv120b_ReadReg(0x04) & 0xf3;
	siv120b_WriteReg(0x04,exposal_time|0x0C);	

				  exposal_time =(siv120b_ReadReg(0x30)<<8) | siv120b_ReadReg(0x31);
	siv120b_WriteReg(0x00,0x01);
	if (exposal_time =1)
		{
		
		siv120b_WriteReg(0x30,0x00);
	      siv120b_WriteReg(0x31,0x01);
		}
	else
		{
		
		exposal_time>>=4;
		if (exposal_time<1)
			{
			exposal_time =1;
			}

		siv120b_WriteReg(0x30,(exposal_time>>8)&0x00FF);
		siv120b_WriteReg(0x31,exposal_time&0x00FF);
			}
			}
		else
				{
				siv120b_WriteReg(0x00, 0x00);
	              siv120b_WriteReg(0x03, 0x02);
	            //  siv120b_WriteReg(0x04, 0x0C);
				  siv120b_WriteReg(0x00, 0x00);
	exposal_time = siv120b_ReadReg(0x04) & 0xf3;
	siv120b_WriteReg(0x04,exposal_time|0x04);	

				 exposal_time =(siv120b_ReadReg(0x30)<<8) | siv120b_ReadReg(0x31);
	siv120b_WriteReg(0x00,0x01);
	if (exposal_time =1)
		{
		
		siv120b_WriteReg(0x30,0x00);
	      siv120b_WriteReg(0x31,0x01);
		}else
		{
		
		exposal_time>>=4;
		if (exposal_time<1)
			{
			exposal_time =1;
			}

		siv120b_WriteReg(0x30,(exposal_time>>8)&0x00FF);
		siv120b_WriteReg(0x31,exposal_time&0x00FF);
		}
			}
	
	


	/*
	reg_val = siv120b_ReadReg(0x31);
	exposal_time=reg_val&0x00ff;
	reg_val = siv120b_ReadReg(0x30);
	exposal_time|=((reg_val&0x00ff)<<0x08);

	exposal_time=exposal_time>>0x01;

	if(exposal_time<1)
	{
		exposal_time=1;
	}

	reg_val=exposal_time&0x00ff;
	siv120b_WriteReg(0x31, reg_val);
	reg_val=(exposal_time&0xff00)>>0x08;
	siv120b_WriteReg(0x30, reg_val);

	siv120b_WriteReg(0x00, 0x00);
	siv120b_WriteReg(0x03, 0xc5);
  	SCI_Sleep(400);
  	*/
/***************1.3M  add below******************************/
	/*
if(TRUE == siv120b_Get_Is_1M())
	{		
 		//BLACK LEVEL CHANGE FUNCTION   
 	    siv120b_WriteReg(0x03, 0x00);
 	    siv120b_WriteReg(0x80, 0x1E);
 	    siv120b_WriteReg(0x03, 0x03);
 	    siv120b_WriteReg(0x50, 0x00);
 	    siv120b_WriteReg(0x51, 0x03);
 	    siv120b_WriteReg(0x52, 0x06); 
 	    siv120b_WriteReg(0x53, 0x0A);
 	    siv120b_WriteReg(0x54, 0x12);
 	    siv120b_WriteReg(0x55, 0x1D);
 	    siv120b_WriteReg(0x56, 0x00);
 	    siv120b_WriteReg(0x57, 0xFF);
		
		siv120b_WriteReg(0x03,0x00);
		siv120b_WriteReg(0x91,0x50);	//clock divide  0x50   
		siv120b_WriteReg(0x03,0x01);
		siv120b_WriteReg(0x48,0x03);	//0x0a
		iExp = iExp/2;               			//10
	}
	else
	{
		siv120b_WriteReg(0x03,0x00);
		siv120b_WriteReg(0x91,0x10);	//clock divide  
		siv120b_WriteReg(0x03,0x01);
		siv120b_WriteReg(0x48,0x03);	//0x0a
		iExp =iExp /1;		// 2
	}
/**************1.3M  add above****************************/
	
	/*
	siv120b_WriteReg(0x03,0x02);
	siv120b_WriteReg(0x2C, ((iExp & 0xFF000000) >> 24));
	siv120b_WriteReg(0x2D, ((iExp & 0x00FF0000) >> 16));
	siv120b_WriteReg(0x2E, ((iExp & 0x0000FF00) >> 8));
	siv120b_WriteReg(0x2F, (iExp & 0x000000FF));

{

   //SCI_TRACE_LOW:" siv120b_before_snapshot 0x2C = %0X"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_1210_112_2_18_0_32_39_1371,(uint8*)"d", siv120b_ReadReg(0x2C));
   //SCI_TRACE_LOW:" siv120b_before_snapshot 0x2D = %0X"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_1211_112_2_18_0_32_39_1372,(uint8*)"d", siv120b_ReadReg(0x2D));
   //SCI_TRACE_LOW:" siv120b_before_snapshot 0x2E = %0X"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_1212_112_2_18_0_32_39_1373,(uint8*)"d", siv120b_ReadReg(0x2E));
   //SCI_TRACE_LOW:" siv120b_before_snapshot 0x2F = %0X"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV120B_1213_112_2_18_0_32_39_1374,(uint8*)"d", siv120b_ReadReg(0x2F));

}
*/

 #endif
	return 0;
   }
                     


LOCAL uint32 siv120b_after_snapshot(uint32 para)
{
#if 0
uint32 iExp;
	siv120b_WriteReg(0x00, 0x00);
	siv120b_WriteReg(0x03, 0x02);
	
	

	siv120b_ae_aeb_enable(0x01, 0x01);
	//siv120b_WriteReg(0x00, 0x00);
	//siv120b_WriteReg(0x20, 0x20);
	//siv120b_WriteReg(0x21, 0xbd);
	//siv120b_WriteReg(0x22, 0xdd);

	siv120b_WriteReg(0x00, 0x01);
	//siv120b_WriteReg(0x34, 0x50);

      siv120b_WriteReg(0x00, 0x00);
	siv120b_WriteReg(0x03, 0xC5);
#endif
    
	return 0;
}


LOCAL SENSOR_REG_TAB_INFO_T s_siv120b_resolution_Tab_YUV[]=
{
    // COMMON INIT
    { NULL,    			 0,	    0,  0,  	       24,              0 },  //12

    // YUV422 PREVIEW 1
    { ADDR_AND_LEN_OF_ARRAY(siv120b_YVU_640X480),    640,    480,   24,  0 }, //12
    { PNULL,                    0,      0,  0,           0,               0 },
    { PNULL,                    0,      0,  0,           0,               0 },
    { PNULL,                    0,      0,  0,           0,               0 },


    // YUV422 PREVIEW 2
    { PNULL,                    0,      0,  0,           0,               0 },
    { PNULL,                    0,      0,  0,           0,               0 },
    { PNULL,                    0,      0,  0,           0,               0 },
    { PNULL,                    0,      0,  0,           0,               0 }

};

LOCAL SENSOR_IOCTL_FUNC_TAB_T s_siv120b_ioctl_func_tab = 
{
// Internal 
    PNULL,  //reset
    PNULL,  //power
    PNULL,  //enter_sleep
    siv120b_identify,  //identify
    PNULL,   // write register
    PNULL,   // read  register   
    PNULL,   //cus_func_1
    PNULL,   //cus_func_2

// External
    PNULL,  //ae_enable
    siv120b_hmirror_enable,  //hmirror_enable
    siv120b_vmirror_enable,  //vmirror_enable 
    siv120b_set_brightness, //set_brightness
    siv120b_set_contrast,    //set_contrast
    PNULL,  //set_sharpness
    PNULL,  //set_saturation
    siv120b_set_preview_mode,  //set_preview_mode
    siv120b_set_image_effect,    //set_image_effect
    siv120b_before_snapshot, //before_snapshort
    siv120b_after_snapshot,   //after_snapshort

    PNULL, //flash
    PNULL, //read_ev_value,
    PNULL, //write_ev_value,
    PNULL, //read_gain_value,
    PNULL, //write_gain_value,
    PNULL, //read_gain_scale,
    PNULL, //set_frame_rate,
    PNULL, //af_enable
    PNULL, //af_get_status
    siv120b_set_awb, //set_wb_mode
    PNULL, //get_skip_frame
    PNULL, //set_iso
    siv120b_set_ev, //set_exposure_compensation
    PNULL,   //check_image_format_support
    PNULL,   //change_image_format
    PNULL,   //cus_func_3
    PNULL,   //cus_func_4
	PNULL,
    siv120b_set_anti_flicker,   //set_anti_banding_flicker
    siv120b_set_video_mode, //set_video_mode
    PNULL,   //pick_jpeg_stream
};



PUBLIC SENSOR_INFO_T g_SIV120B_yuv_info =
{
	siv120b_I2C_ADDR_W,				// salve i2c write address
	siv120b_I2C_ADDR_R, 				// salve i2c read address

	0,								// bit0: 0: i2c register value is 8 bit, 1: i2c register value is 16 bit
									// bit2: 0: i2c register addr  is 8 bit, 1: i2c register addr  is 16 bit
									// other bit: reseved
	SENSOR_HW_SIGNAL_PCLK_P|\
	SENSOR_HW_SIGNAL_VSYNC_N|\
	SENSOR_HW_SIGNAL_HSYNC_P,		// bit0: 0:negative; 1:positive -> polarily of pixel clock
									// bit2: 0:negative; 1:positive -> polarily of horizontal synchronization signal
									// bit4: 0:negative; 1:positive -> polarily of vertical synchronization signal
									// other bit: reseved

	// preview mode
	SENSOR_ENVIROMENT_NORMAL|\
	SENSOR_ENVIROMENT_NIGHT|\
	SENSOR_ENVIROMENT_SUNNY,

	// image effect
	SENSOR_IMAGE_EFFECT_NORMAL|\
	SENSOR_IMAGE_EFFECT_BLACKWHITE|\
	SENSOR_IMAGE_EFFECT_RED|\
	SENSOR_IMAGE_EFFECT_GREEN|\
	SENSOR_IMAGE_EFFECT_BLUE|\
	SENSOR_IMAGE_EFFECT_YELLOW|\
	SENSOR_IMAGE_EFFECT_NEGATIVE|\
	SENSOR_IMAGE_EFFECT_CANVAS,

	// while balance mode
	0,

	7,								// bit[0:7]: count of step in brightness, contrast, sharpness, saturation
									// bit[8:31] reseved

	SENSOR_LOW_PULSE_RESET,		// reset pulse level
	100,								// reset pulse width(ms)

	SENSOR_LOW_LEVEL_PWDN,			// 1: high level valid; 0: low level valid

	2,								// count of identify code
	0x01, 0x12,						// supply two code to identify sensor.
	0x02, 0x11,						// for Example: index = 0-> Device id, index = 1 -> version id

	SENSOR_AVDD_2800MV,				// voltage of avdd

	640,							// max width of source image
	480,							// max height of source image
	"siv120b",						// name of sensor

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;

	s_siv120b_resolution_Tab_YUV,	// point to resolution table information structure
	&s_siv120b_ioctl_func_tab,		// point to ioctl function table

	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor

	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	0,							// skip frame num before preview;
	0,							// skip frame num before capture;	
	0,                      // deci frame num during preview	
	2,                       // deci frame num during video preview	

	0,	// threshold enable		
	0,  // threshold mode 	
	0,	// threshold start postion	
	0,	// threshold end postion 
	0
};


#endif
