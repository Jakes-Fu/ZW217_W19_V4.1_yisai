/******************************************************************************
 ** File Name:      SIV100B.c                                         *
 ** Author:        	cloud6                                             	  *
 ** Date:           07/09/2007                                            *
 ** Copyright:      XUNRUI. All Rights Reserved.        *
 ** Description:    implementation of digital camera register interface       *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **                                  *
 ******************************************************************************/

#ifndef _SIV100B_C_
#define _SIV100B_C_

#include "ms_customize_trc.h"
#include "sensor_cfg.h"
#include "sensor_drv.h"
#include "os_api.h"
#include "chip.h"
#include "dal_dcamera.h"


/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
#define SIV100B_I2C_ADDR_W    			0x66
#define SIV100B_I2C_ADDR_R    			0x67
#define SIV100B_I2C_ACK				0x0

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
#define    SIV100B_DELAY_MS(ms)            {\
                                              uint32 end = SCI_GetTickCount() + ms;\
                                              while (SCI_GetTickCount() < end)\
                                              ;\
                                          }
                                           
 


/**---------------------------------------------------------------------------*
 **                         Const variables                                   *
 **---------------------------------------------------------------------------*/
// OV give the parameters

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/


__align(4) const SENSOR_REG_T siv100b_YVU_640X480[] = 
{
	{0x04, 0x03},//0x03
	{0x11, 0x04},
	{0x12, 0x0a},
	{0x13, 0x1f},
	{0x16, 0x89},
	{0x1b, 0x90},
	{0x1f, 0x52},
	/*26M 20FPS*/
	{0x20, 0x00},//0x11
	{0x21, 0xa0},//0x0c
	{0x22, 0x7d},//0x72
	{0x34, 0x7c},//0x7b
	{0x33, 0x08},//0x10
	{0x40, 0x80},//0x84
	{0x41, 0x88},
	{0x42, 0x7f},
	{0x43, 0xc0},
	{0x44, 0x38},
	{0x45, 0x2c},
	{0x46, 0x0c},
	{0x47, 0x15},
	{0x48, 0x20},
	{0x49, 0x10},
	{0x4a, 0x84},
	{0x4b, 0x82},
	{0x4c, 0x3c},
	{0x4e, 0x17},
	{0x4f, 0x8a},
	{0x50, 0x94},
	{0x5a, 0x00},
	{0x60, 0xc3},
	{0x61, 0x88},
	{0x62, 0x01},
	{0x63, 0x80},
	{0x64, 0x78},//0x80
	{0x65, 0xd2},
	{0x66, 0x93},
	{0x67, 0xd6},
	{0x68, 0x90},
	{0x69, 0x8a},
	{0x6a, 0x73},
	{0x6b, 0x95},
	{0x6c, 0x70},
	{0x6d, 0x88},
	{0x6e, 0x77},
	{0x6f, 0x46},
	{0x70, 0xea},
	{0x71, 0x60},
	{0x72, 0x05},
	{0x73, 0x02},
	{0x74, 0x0c},
	{0x75, 0x0f},
	{0x76, 0x20},
	{0x77, 0xb7},
	{0x78, 0x97},
	{0x79, 0x80},
	{0x7c, 0x85},
	{0x80, 0xaf},
	{0x81, 0x9d},//	{0x81, 0x8d},
	{0x82, 0x3d},
	{0x83, 0x00},
	{0x85, 0xff},//jing.li
	{0x86, 0xa1},
	{0x87, 0x04},//0x10
	{0x88, 0x24},//0x2a
	{0x89, 0x0f},
	{0x92, 0x44},
	{0x93, 0x0c},
	{0x94, 0x20},
	{0x95, 0x20},
	{0x96, 0x0c},
	{0x97, 0x20},
	{0x98, 0x20},
	{0x99, 0x18},
	{0x9a, 0x30},
	{0xa4, 0xdd},
	{0xa5, 0xdd},
	{0xa6, 0xdd},
	{0xa7, 0xdc},
	{0xa8, 0xba},
	{0xa9, 0x54},
	{0xaa, 0x36},
	{0xab, 0x21},
	{0xac, 0x03},
	{0xad, 0x10},
	{0xae, 0x00},
	{0xaf, 0x98},
	{0xb0, 0x80},
	{0xb1, 0x00},
	{0xb2, 0x0d},
	{0xb3, 0x16},
	{0xb4, 0x2a},
	{0xb5, 0x48},
	{0xb6, 0x64},
	{0xb7, 0x79},
	{0xb8, 0x8c},
	{0xb9, 0x9c},
	{0xba, 0xAa},
	{0xbb, 0xB6},
	{0xbc, 0xcb},
	{0xbd, 0xde},
	{0xbe, 0xF0},
	{0xbf, 0xF8},
	{0xc0, 0xFF},
	{0xc1, 0x3a},
	{0xc2, 0xcb},
	{0xc3, 0xfb},
	{0xc4, 0x10},
	{0xc5, 0x1d},
	{0xc6, 0x13},
	{0xc7, 0xf3},
	{0xc8, 0xbd},
	{0xc9, 0x50},
	{0xca, 0x90},
	{0xcb, 0x20},//0x0e
	{0xcc, 0x40},//0x27
	{0xcd, 0x04},
	{0xce, 0x04},
	{0xcf, 0x20},
	{0xd0, 0x20},
	{0xd1, 0x2a},
	{0xd2, 0x86},
	{0xd3, 0x00},
	{0xd4, 0x10},
	{0xd5, 0x13},
	{0xd6, 0x13},
	{0xd7, 0x00},

	{0xdb, 0xff},
	{0xdc, 0x00},//0x00
	{0xe1, 0x28},
	{0xe2, 0x8a},//0x2b,
	{0x40, 0x80},
	{0x03, 0xc5},

};



/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
LOCAL uint8 SIV100B_WriteReg( uint8  subaddr, uint8 data )
{      
	#ifndef	_USE_DSP_I2C_
		//uint8 cmd[2];
		//cmd[0]	=	subaddr;
		//cmd[1]	=	data;
		
		//I2C_WriteCmdArr(SIV100B_I2C_ADDR_W, cmd, 2, SCI_TRUE);
		Sensor_WriteReg(subaddr,data);
	#else
		DSENSOR_IICWrite((uint16)subaddr, (uint16)data);
	#endif

	//SCI_TRACE_LOW:"SENSOR: SIV100B_WriteReg reg/value(%x,%x) !!"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_206_112_2_18_0_32_35_1338,(uint8*)"dd", subaddr, data);

    return SCI_SUCCESS;
}

LOCAL uint8 SIV100B_ReadReg( uint8  subaddr)
{
	uint8 value = 0;
	
	#ifndef	_USE_DSP_I2C_
		//I2C_WriteCmdArr(SIV100B_I2C_ADDR_W, &subaddr, 1, SCI_TRUE);
		//I2C_ReadCmd(SIV100B_I2C_ADDR_R, &value, SCI_TRUE);
		value=Sensor_ReadReg( subaddr);
		//SCI_TRACE_LOW:"SIV100B read ret is %X=%X"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_219_112_2_18_0_32_35_1339,(uint8*)"dd",subaddr,value );
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
LOCAL uint32 SIV100B_Identify(uint32 param)
{
    uint8 reg[2]    = {0x01, 0x02};
    uint8 value[2]  = {0x0c, 0x11};
    uint8 ret       = 0;
    uint32 i;
    uint8 err_cnt = 0;
    uint32 nLoop = 1000;
    
    for(i = 0; i<2; )
    {
        nLoop = 1000;
        ret = SIV100B_ReadReg(reg[i]);    
		
		//SCI_TRACE_LOW:"SIV100B read ret is %X"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_249_112_2_18_0_32_35_1340,(uint8*)"d",ret );
		
        if( ret != value[i])
        {
            err_cnt++;
            if(err_cnt>3)
            {
            	//SCI_TRACE_LOW:"SIV100B Fail"
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_256_112_2_18_0_32_35_1341,(uint8*)"" );
                return 0xFF;
            }
            else
            {
                //Masked by frank.yang,SCI_Sleep() will cause a  Assert when called in boot precedure
                //SCI_Sleep(10);
                while(nLoop--){};
                continue;
            }
        }
        err_cnt = 0;
        i++;
    }

     //SCI_TRACE_LOW:"SIV100B succ"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_271_112_2_18_0_32_35_1342,(uint8*)"" );
//    s_sensor_ID = SENSOR_SIV100B;
    return SCI_SUCCESS; 
}

__align(4) const SENSOR_REG_T siv100b_brightness_tab[][2]=
{
    {
//		{0x03, 0x02},		
		{0xd7,0xe0}, 
//		{0x03, 0xc5},		
		{0xff,0xff},
	},

	{
//		{0x03, 0x02},	
		{0xd7,0xc8}, 
//		{0x03, 0xc5},		
		{0xff,0xff},
	},

	{
//		{0x03, 0x02},	
		{0xd7,0xb0},
//		{0x03, 0xc5},		
		{0xff,0xff},
	},

	{
//		{0x03, 0x02},	
		{0xd7,0x98},
//		{0x03, 0xc5},		
		{0xff,0xff},
	},

	{
//		{0x03, 0x02},	
		{0xd7,0x00}, 
//		{0x03, 0xc5},		
		{0xff,0xff},
	},

	{
//		{0x03, 0x02},	
		{0xd7,0x18},
//		{0x03, 0xc5},		
		{0xff,0xff},
	},

	{
//		{0x03, 0x02},	
		{0xd7,0x30},
//		{0x03, 0xc5},		
		{0xff,0xff},
	}
};
LOCAL uint32 set_brightness(uint32 level)
{
    uint16 i;
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv100b_brightness_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_brightness: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_339_112_2_18_0_32_35_1343,(uint8*)"d", level);
    
    return 0;
}


__align(4) const SENSOR_REG_T siv100b_contrast_tab[][2]=
{
    {
            {0xd4, 0x06},
            {0xFF, 0xFF},
    },

    {
            {0xd4, 0x09},
            {0xFF, 0xFF},
    },

    {
            {0xd4, 0x0c},
            {0xFF, 0xFF},
    },

    {
            {0xd4, 0x10},
            {0xFF, 0xFF},
    },

    {
            {0xd4, 0x13},
            {0xFF, 0xFF},
    },    
    {
            {0xd4, 0x16},
            {0xFF, 0xFF},
    },    
    {
            {0xd4, 0x19},
            {0xFF, 0xFF},
    },                   
};

LOCAL uint32 set_contrast(uint32 level)
{
    uint16 i;    
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv100b_contrast_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_393_112_2_18_0_32_35_1344,(uint8*)"d", level);
    
    return 0;
}



__align(4) const SENSOR_REG_T siv100b_ev_tab[][2]=
{

#if 0
    {
            {0x41, 0x5a},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x6a},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x7a},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x88},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x9a},
            {0xFF, 0xFF},
    },    
    {
            {0x41, 0xaa},
            {0xFF, 0xFF},
    },    
    {
            {0x41, 0xba},
            {0xFF, 0xFF},
    },                   
 #else

   {
            {0x41, 0x58},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x68},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x78},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x84},
            {0xFF, 0xFF},
    },

    {
            {0x41, 0x94},
            {0xFF, 0xFF},
    },    
    {
            {0x41, 0xa4},
            {0xFF, 0xFF},
    },    
    {
            {0x41, 0xb4},
            {0xFF, 0xFF},
    },                   
 #endif
};

LOCAL uint32 set_ev(uint32 level)
{
    uint16 i;    


    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv100b_ev_tab[level];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

     if( (level+1) > sizeof(siv100b_ev_tab)/(2*sizeof(SENSOR_REG_T)) )
    {
    	   SCI_ASSERT(0);
    }
    
    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }
    //SCI_TRACE_LOW:"set_contrast: level = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_491_112_2_18_0_32_35_1345,(uint8*)"d", level);
    
    return 0;
}



LOCAL uint32 set_anti_flicker(uint32 level)
{
    uint16 i;    

    
    return 0;
}




LOCAL uint32 set_awb(uint32 mode)
{
	DCAMERA_PARAM_WB_MODE_E awb_mode = (DCAMERA_PARAM_WB_MODE_E)mode;

	switch(awb_mode)
	{
		case DCAMERA_WB_MODE_INCANDESCENCE:

			 //SCI_TRACE_LOW:"SIV100B: DCAMERA_WB_MODE_INCANDESCENCE"
			 SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_517_112_2_18_0_32_35_1346,(uint8*)"");

			 Sensor_WriteReg(0x60, 0x00);
			 Sensor_WriteReg(0x7a, 0x98);
 			 Sensor_WriteReg(0x7b, 0xc8);
		break;

		case DCAMERA_WB_MODE_FLUORESCENT:

			 //SCI_TRACE_LOW:"SIV100B: DCAMERA_WB_MODE_FLUORESCENT"
			 SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_526_112_2_18_0_32_35_1347,(uint8*)"");

			 Sensor_WriteReg(0x60, 0x00);
			 Sensor_WriteReg(0x7a, 0xaa);
 			 Sensor_WriteReg(0x7b, 0xbe);
		break;

		case DCAMERA_WB_MODE_SUN:

			//SCI_TRACE_LOW:"SIV100B: DCAMERA_WB_MODE_SUN"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_535_112_2_18_0_32_35_1348,(uint8*)"");

			 Sensor_WriteReg(0x60, 0x00);
			 Sensor_WriteReg(0x7a, 0xc2);
 			 Sensor_WriteReg(0x7b, 0x9e);

		break;

		case DCAMERA_WB_MODE_CLOUD:

			//SCI_TRACE_LOW:"SIV100B: DCAMERA_WB_MODE_CLOUD"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_545_112_2_18_0_32_35_1349,(uint8*)"");
			 Sensor_WriteReg(0x60, 0x90);
			 Sensor_WriteReg(0x7a, 0xd0);
 			 Sensor_WriteReg(0x7b, 0x88);

		break;

              case  DCAMERA_WB_MODE_AUTO:


		       //SCI_TRACE_LOW:"SIV100B: DCAMERA_WB_MODE_AUTO"
		       SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_555_112_2_18_0_32_35_1350,(uint8*)"");

			 Sensor_WriteReg(0x60, 0x90);
			 Sensor_WriteReg(0x7a, 0xaa);
 			 Sensor_WriteReg(0x7b, 0xbe);

		break;

		default:

		        SCI_ASSERT(0);

		break;
		
	}
        return 0;
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

__align(4) const SENSOR_REG_T siv100b_mode_tab[][1]=
{
	//normal
    {
            {0xFF, 0xFF},
    },
	//night
    {
            {0xFF, 0xFF},
    }
};

LOCAL uint32 siv100b_set_video_mode(uint32 video_mode)
{

    Sensor_WriteReg(0x40, 0x00);
     SCI_Sleep(200);
    if(video_mode)// video
    {
    	 //SCI_TRACE_LOW:"siv100b_set_video_mode 1"
    	 SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_603_112_2_18_0_32_35_1351,(uint8*)"");
        Sensor_WriteReg(0x03, 0x02);
        Sensor_WriteReg(0x20, 0x00);
        Sensor_WriteReg(0x21, 0x2d);
        Sensor_WriteReg(0x22, 0x41);
        Sensor_WriteReg(0x34, 0x8e);
        Sensor_WriteReg(0x33, 0x04);
        Sensor_WriteReg(0x03, 0xc5);

    }
    else // nomal
    {
    //SCI_TRACE_LOW:"siv100b_set_video_mode 0"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_615_112_2_18_0_32_35_1352,(uint8*)"");
        Sensor_WriteReg(0x03, 0x02);
        Sensor_WriteReg(0x20, 0x00);
        Sensor_WriteReg(0x21, 0x2d);
        Sensor_WriteReg(0x22, 0x41);
        Sensor_WriteReg(0x34, 0x8e);
        Sensor_WriteReg(0x33, 0x0a);
        Sensor_WriteReg(0x03, 0xc5);		
    }

    
    SCI_Sleep(200);
    Sensor_WriteReg(0x40, 0x80);
    
    return 0;
}


LOCAL uint32 set_preview_mode(uint32 preview_mode)
{
     Sensor_WriteReg(0x40, 0x00);
     SCI_Sleep(200);//disable AE; and wait for AE Stable
    if(preview_mode)// night
    {
        //SCI_TRACE_LOW:"set_preview_mode 1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_639_112_2_18_0_32_35_1353,(uint8*)"");
        Sensor_WriteReg(0x03, 0x02);
        Sensor_WriteReg(0x20, 0x00);
        Sensor_WriteReg(0x21, 0x2d);
        Sensor_WriteReg(0x22, 0x41);
        Sensor_WriteReg(0x34, 0x8e);
        Sensor_WriteReg(0x33, 0x14);
        Sensor_WriteReg(0x03, 0xc5);
    }
    else // nomal
    {

        //SCI_TRACE_LOW:"set_preview_mode 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_651_112_2_18_0_32_35_1354,(uint8*)"");
        Sensor_WriteReg(0x03, 0x02);
        Sensor_WriteReg(0x20, 0x00);
        Sensor_WriteReg(0x21, 0x2d);
        Sensor_WriteReg(0x22, 0x41);
        Sensor_WriteReg(0x34, 0x8e);
        Sensor_WriteReg(0x33, 0x0a);
        Sensor_WriteReg(0x03, 0xc5);		
     
    }

    SCI_Sleep(200);//Enable AE; and wait for AE Stable
    Sensor_WriteReg(0x40, 0x80);
	
    SCI_Sleep(500);
    return 0;
}




__align(4) const SENSOR_REG_T siv100b_image_effect_tab[][4]=
{
	 //const uint8 SIV100B_SENSOR_SEPIA_NORMAL [][3] =
	{
		{0xd8,0x00},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}  
	},

	//const uint8 SIV100B_SENSOR_SEPIA_BLACKWHITE [][3] =
	{
		{0xd8,0x40},
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}        
	},

	//const uint8 SIV100B_SENSOR_SEPIA_BLUE [][3] =
	{

		{0xd8,0x80},
		{0xd9,0x60},
		{0xda,0xe0},
		{0xff,0xff}
	},
	
	//const uint8 SIV100B_SENSOR_SEPIA_GREEN [][3] =
	{
		{0xd8,0x80},
		{0xd9,0x50},
		{0xda,0x50},
		{0xff,0xff}
	},

	//const uint8 SIV100B_SENSOR_SEPIA_RED [][3] =
	{
		{0xd8,0x80},
		{0xd9,0xc0},
		{0xda,0x60},
		{0xff,0xff}
	},
	
	//const uint8 SIV100B_SENSOR_SEPIA_CANVAS [][3] =
	{

		{0xd8,0x80},
		{0xd9,0x00},
		{0xda,0xa0},
		{0xff,0xff}
	},

      	//const uint8 SIV100B_SENSOR_COLOR_INV [][3] =
	{
		{0xd8,0x10}, 
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}  
	},
	//const uint8 SIV100B_SENSOR_SEPIA_NEGATIVE [][3] =
	{

		{0xd8,0x80},
		{0xd9,0x60},
		{0xda,0xa0},
		{0xff,0xff}
	},

	//const uint8 SIV100B_SENSOR_GRAY_INV [][3] =
	{
		{0xd8,0x20}, 
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}  
	},

	//const uint8 SIV100B_SENSOR_EMBOSSMENT [][3] =
	{
		{0xd8,0x08}, 
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}  
	},

	//const uint8 SIV100B_SENSOR_SKETCH [][3] =
	{
		{0xd8,0x04}, 
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}  
	},

	//const uint8 SIV100B_SENSOR_SEPIA_MAGENTA [][3] =
	{
		{0xd8,0x80},
		{0xd9,0x00},
		{0xda,0xA0},
		{0xff,0xff},
	},

	//const uint8 SIV100B_VFLIP_ENABLE[][3] = 
	{
		{0x04,0x02}, 
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}  
	},

	//const uint8 SIV100B_VFLIP_DISABLE[][3] = 
	{
		{0x04,0x00}, 
		{0xff,0xff},
		{0xff,0xff},
		{0xff,0xff}  
    },

};

LOCAL uint32 set_image_effect(uint32 effect_type)
{
    uint16 i;
    
    
    SENSOR_REG_T* sensor_reg_ptr = (SENSOR_REG_T*)siv100b_image_effect_tab[effect_type];

    SCI_ASSERT(PNULL != sensor_reg_ptr);

    for(i = 0; (0xFF != sensor_reg_ptr[i].reg_addr) && (0xFF != sensor_reg_ptr[i].reg_value) ; i++)
    {
        Sensor_WriteReg(sensor_reg_ptr[i].reg_addr, sensor_reg_ptr[i].reg_value);
    }

    //SCI_TRACE_LOW:"-----------set_image_effect: effect_type = %d------------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,SENSOR_SIV100B_804_112_2_18_0_32_36_1355,(uint8*)"d", effect_type);
    
    return 0;
}

LOCAL uint32 siv100b_ae_aeb_enable(uint32 ae_enable, uint32 awb_enable)
{
	uint16 ae_value = 0;

	ae_value = SIV100B_ReadReg(0x40);

	if(0x01==ae_enable)
	{
		ae_value|=0x80;
	}
	else if(0x00==ae_enable)
	{
		ae_value&=0x7f;
	}

	if(0x01==awb_enable)
	{

	}
	else if(0x00==awb_enable)
	{

	}	
	
	SIV100B_WriteReg(0x40, ae_value);

	return 0;
}

LOCAL uint32 siv100b_before_snapshot(uint32 para)
{
	uint8 reg_val = 0;
	uint16 exposal_time=0x00;

	siv100b_ae_aeb_enable(0x00, 0x00);

	SIV100B_WriteReg(0x03, 0x02);
	SIV100B_WriteReg(0x04, 0x07);

	reg_val = SIV100B_ReadReg(0x31);
	exposal_time=reg_val&0x00ff;
	reg_val = SIV100B_ReadReg(0x30);
	exposal_time|=((reg_val&0x00ff)<<0x08);
	
	exposal_time=exposal_time>>0x01;

	if(exposal_time<1)
	{
		exposal_time=1;
	}

	reg_val=exposal_time&0x00ff;
	SIV100B_WriteReg(0x31, reg_val);
	reg_val=(exposal_time&0xff00)>>0x08;
	SIV100B_WriteReg(0x30, reg_val);
	SIV100B_WriteReg(0x03, 0xc5);	
  	SCI_Sleep(400); 	
	

	return 0;
}

LOCAL uint32 siv100b_after_snapshot(uint32 para)
{
	SIV100B_WriteReg(0x03, 0x02);

	siv100b_ae_aeb_enable(0x01, 0x00);

	SIV100B_WriteReg(0x04, 0x03);

	SIV100B_WriteReg(0x03, 0xc5);	

	return 0;
}


LOCAL SENSOR_REG_TAB_INFO_T s_siv100b_resolution_Tab_YUV[]=
{
	// COMMON INIT
	{ADDR_AND_LEN_OF_ARRAY(siv100b_YVU_640X480), 640, 480, 24, SENSOR_IMAGE_FORMAT_YUV422},
	
	// YUV422 PREVIEW 1	
	{PNULL, 0, 640, 480,24, SENSOR_IMAGE_FORMAT_YUV422},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	
	// YUV422 PREVIEW 2 
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0},
	{PNULL, 0, 0, 0, 0, 0}
};


LOCAL SENSOR_IOCTL_FUNC_TAB_T s_siv100b_ioctl_func_tab = 
{
// Internal 
    PNULL,
    PNULL,
    PNULL,
    SIV100B_Identify,  
    PNULL,          // write register
    PNULL,          // read  register   
    PNULL,
    PNULL,

// External
    PNULL,
    PNULL,
    PNULL,    
    set_brightness,
    set_contrast, 
    PNULL, 
    PNULL, 
    set_preview_mode, 
    set_image_effect, 
    siv100b_before_snapshot, 
    siv100b_after_snapshot,

    PNULL,
    PNULL, //read_ev_value,
    PNULL, //write_ev_value,
    PNULL, //read_gain_value,
    PNULL, //write_gain_value,
    PNULL, //read_gain_scale,
    PNULL, //set_frame_rate,
    PNULL,
    PNULL,
    set_awb,
    PNULL,
    PNULL,
    set_ev,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    PNULL,
    siv100b_set_video_mode,
    PNULL
};


PUBLIC SENSOR_INFO_T g_SIV100B_yuv_info =
{
	SIV100B_I2C_ADDR_W,				// salve i2c write address
	SIV100B_I2C_ADDR_R, 				// salve i2c read address
	
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
	
	SENSOR_HIGH_LEVEL_PWDN,			// 1: high level valid; 0: low level valid
	
	2,								// count of identify code
	0x01, 0x0c,						// supply two code to identify sensor.
	0x02, 0x11,						// for Example: index = 0-> Device id, index = 1 -> version id											
											
	SENSOR_AVDD_2800MV,				// voltage of avdd	
	
	640,							// max width of source image
	480,							// max height of source image
	"SIV100B",						// name of sensor												

	SENSOR_IMAGE_FORMAT_YUV422,		// define in SENSOR_IMAGE_FORMAT_E enum,
									// if set to SENSOR_IMAGE_FORMAT_MAX here, image format depent on SENSOR_REG_TAB_INFO_T
	SENSOR_IMAGE_PATTERN_YUV422_YUYV,	// pattern of input image form sensor;			

	s_siv100b_resolution_Tab_YUV,	// point to resolution table information structure
	&s_siv100b_ioctl_func_tab,		// point to ioctl function table
			
	PNULL,							// information and table about Rawrgb sensor
	PNULL,							// extend information about sensor	

	SENSOR_AVDD_2800MV,                     // iovdd
	SENSOR_AVDD_1800MV,                      // dvdd
	0,							// skip frame num before preview;
	0,							// skip frame num before capture;	
	2,                      // deci frame num during preview	
	2,                       // deci frame num during video preview	
	0,                     // threshold enable(only analog TV)	
	0,                       // atv output mode 0 fix mode 1 auto mode	
	0,                      // atv output start postion	
	0,     	                 // atv output end postion     
	0
};


#endif

